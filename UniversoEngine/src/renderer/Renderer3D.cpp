#include "Renderer3D.h"

#include <algorithm>
#include <cstring>
#include <string>

#include "../debug/Assert.h"

namespace engine {

	Renderer3D::Renderer3D() {
		vertices = new Vertex[maxVertices];
		verticesBegin = vertices;
		indices = new unsigned int[maxIndices];

		vertexArray.addVertexBuffer(vertexBuffer);
		vertexArray.addIndexBuffer(indexBuffer);

		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, position));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, normal));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, ambient));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, diffuse));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, specular));
		vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, shininess));
		vertexBuffer.addAttributePointer(AttriuteType::Vec2, offsetof(Vertex, textureCoords));
		vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, textureSlotIndex));

		fragShader.addMacroDefinition("MAX_POINT_LIGHTS",       std::to_string(PointLight::maxPointLights));
		fragShader.addMacroDefinition("MAX_DIRECTIONAL_LIGHTS", std::to_string(DirectionalLight::maxDirectionalLights));
		fragShader.addMacroDefinition("MAX_TEXTURES",           std::to_string(Texture::maxTextures));
		fragShader.addMacroDefinition("MAX_CUBE_MAPS",          std::to_string(Texture::maxCubeMaps));

		shaderProgram.attachShader(vertexShader);
		shaderProgram.attachShader(fragShader);
		shaderProgram.bind();
		
		depthshaderProgram.attachShader(depthVertexShader);
		depthshaderProgram.attachShader(depthFragShader);

		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapVertexShader);
		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapGeometryShader);
		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapFragmentShader);

	}

	Renderer3D::~Renderer3D() {
		delete[] this->verticesBegin;
		delete[] this->indices;
	}

	void Renderer3D::startDrawing(Camera& camera) {
		int currentViewPortWidth = DrawApi::getViewPortWidth();
		int currentViewPortHeight = DrawApi::getViewPortHeight();

		CameraUniformBufferData cameraUniformBufferData{ 
			camera.getViewProjectionMatrix(currentViewPortWidth, currentViewPortHeight),
			camera.getViewMatrix(),
			camera.getProjectionMatrix(currentViewPortWidth, currentViewPortHeight),
			camera.position
		};

		cameraUniformBuffer.pushData(&cameraUniformBufferData, sizeof(CameraUniformBufferData));
	}

	void Renderer3D::endDrawing() {
		this->performDrawCall();
		
		// unbind all the textures/ cubemaps
		this->clearBindedTextures();
		this->clearBindedCubeMaps();

		if (this->cubeMapSkyBox != nullptr)
			this->drawCubeMapSkyBox();
	}

	void Renderer3D::drawMesh(Mesh* mesh, Material* material, glm::mat4 transform) {
		ASSERT(mesh->getVertexCount() <= maxVertices, "Mesh does not fit in the drawcall");

		if (mesh->getVertexCount() + this->vertexCount > maxVertices) {
			this->performDrawCall();
		}
		
		this->bindTexture(material->getTexture());

		Texture* texture = material->getTexture();

		for (const Vertex& vertex : mesh->getVertices()) {
			this->vertices->position = transform * glm::vec4(vertex.position, 1.0f);
			this->vertices->normal = glm::mat3(glm::transpose(glm::inverse(transform))) * vertex.normal;
			this->vertices->ambient = material->ambient;
			this->vertices->diffuse = material->diffuse;
			this->vertices->specular = material->specular;
			this->vertices->shininess = material->shininess;
			this->vertices->textureCoords = vertex.textureCoords;
			this->vertices->textureSlotIndex = texture != nullptr ? texture->getSlot() : -1.0f;
			this->vertices++;
		}

		int indexOffset = this->vertexCount;
		for (int index : mesh->getIndices()) {
			this->indices[this->indexCount] = indexOffset + index;
			this->indexCount++;
		}
		this->vertexCount += mesh->getVertexCount();
	}

	void Renderer3D::startLightsDrawing() {
		this->drawCallsCount = 0;

		// clear directional lights shadow maps
		for (auto& light : directionalLights) {
			light.depthMapFrameBuffer->bind();
			DrawApi::clearDepthBuffer();
			light.depthMapFrameBuffer->unbind();
		}

		//clear point lights shadow maps
		for (auto& light : pointLights) {
			light.depthMapFrameBuffer->bind();
			DrawApi::clearDepthBuffer();
			light.depthMapFrameBuffer->unbind();
		}

		// clear the lights vectors
		pointLights.clear();
		directionalLights.clear();
	}

	void Renderer3D::endLightsDrawing() {
		this->performShadowMapDrawCalls();

		LightsUniformBufferData lightsUniformBufferData;

		lightsUniformBufferData.numberOfPointLights = pointLights.size();
		lightsUniformBufferData.numberOfDirectionalLights = directionalLights.size();

		for (int i = 0; i < this->pointLights.size(); i++)
			lightsUniformBufferData.pointLights[i] = this->pointLights[i].getData();

		for (int i = 0; i < this->directionalLights.size(); i++)
			lightsUniformBufferData.directionalLights[i] = this->directionalLights[i].getData();
		
		this->lightsUniformBuffer.pushData(&lightsUniformBufferData, sizeof(LightsUniformBufferData));
	}

	void Renderer3D::drawMeshShadowMap(Mesh* mesh, glm::mat4 transform) {

		ASSERT(mesh->getVertexCount() <= maxVertices, "Mesh does not fit in the drawcall");

		if (mesh->getVertexCount() + this->vertexCount > maxVertices) {
			this->performShadowMapDrawCalls();
		}

		for (const Vertex& vertex : mesh->getVertices()) {
			this->vertices->position = transform * glm::vec4(vertex.position, 1.0f);
			this->vertices++;
		}

		int indexOffset = this->vertexCount;
		for (int index : mesh->getIndices()) {
			this->indices[this->indexCount] = indexOffset + index;
			this->indexCount++;
		}
		this->vertexCount += mesh->getVertexCount();
	}

	void Renderer3D::drawPointLight(PointLight pointLight, glm::mat4 transform) {
		ASSERT(this->pointLights.size() + 1 <= PointLight::maxPointLights, "Maximum point lights exceded");

		pointLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->pointLights.push_back(pointLight);

		bindCubeMap(pointLight.depthMapCubeMap.get());
	}

	void Renderer3D::drawDirectionalLight(DirectionalLight directionalLight, Camera& camera, glm::mat4 transform) {
		ASSERT(this->directionalLights.size() + 1 <= DirectionalLight::maxDirectionalLights, "Maximum directional lights exceded");

		glm::vec3 lightPosition = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		auto lightDirection = glm::vec3(0.0f, 0.0f, 0.0f) - lightPosition;

		directionalLight.position = camera.front - lightDirection;
		directionalLight.lookAt = camera.front;

		this->directionalLights.push_back(directionalLight);

		bindTexture(directionalLight.depthMapTexture.get());
	}

	void Renderer3D::clearColor(float r, float g, float b, float a) {
		DrawApi::clearColor(r, g, b, a);
	}

	void Renderer3D::setViewPortSize(int width, int height) {
		DrawApi::setViewPortSize(width, height);
	}

	unsigned int Renderer3D::getDrawCallsCount() {
		return this->drawCallsCount;
	}

	void Renderer3D::drawCubeMapSkyBox() {
		DrawApi::setDepthFunctionToLessOrEqual();
		DrawApi::enableDepthMask(false);
		this->cubeMapSkyBox->shaderProgram.bind();
		this->cubeMapSkyBox->vertexArray.bind();
		this->cubeMapSkyBox->cubeMap->bind(0);
		DrawApi::draw(36);
		this->drawCallsCount++;
		DrawApi::enableDepthMask(true);
		DrawApi::setDepthFunctionToLess();
	}

	void Renderer3D::bindUniformBuffers() {
		cameraUniformBuffer.bind(0);
		lightsUniformBuffer.bind(1);
		currentPointLightUniformBuffer.bind(2);
		currentDirectionalLightUniformBuffer.bind(3);
	}

	void Renderer3D::bindTexture(Texture* texture) {
		ASSERT(currentTextureSlot < Texture::maxTextures, "Maximum texture slot exceded");

		if (texture == nullptr) return;

		bool textureBinded = std::find(this->bindedTextures.begin(), this->bindedTextures.end(), texture)
			!= this->bindedTextures.end();

		if (!textureBinded) {
			texture->bind(this->currentTextureSlot);
			this->bindedTextures.push_back(texture);
			this->currentTextureSlot++;
		}
	}

	void Renderer3D::bindCubeMap(Texture* cubeMap) {
		ASSERT(currentCubeMapSlot < Texture::maxCubeMaps, "Maximum cube map slot exceded");

		if (cubeMap == nullptr) return;

		bool cubeMapBinded = std::find(this->bindedCubeMaps.begin(), this->bindedCubeMaps.end(), cubeMap)
			!= this->bindedCubeMaps.end();

		if (!cubeMapBinded) {
			cubeMap->bind(this->currentCubeMapSlot + Texture::maxTextures);
			this->bindedCubeMaps.push_back(cubeMap);
			this->currentCubeMapSlot++;
		}
	}

	void Renderer3D::clearBindedTextures() {
		this->bindedTextures.clear();
		this->currentTextureSlot = 0;
	}

	void Renderer3D::clearBindedCubeMaps() {
		this->bindedCubeMaps.clear();
		this->currentCubeMapSlot = 0;
	}

	void Renderer3D::performDrawCall() {
		this->bindUniformBuffers();
		this->vertexArray.bind();
		this->shaderProgram.bind();

		// upload the data to vertex/index buffer in the gpu
		this->vertexBuffer.pushData(this->verticesBegin, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer.pushData(this->indices, sizeof(unsigned int) * this->indexCount);

		DrawApi::drawWithIdexes(this->indexCount);

		this->vertices = this->verticesBegin;
		this->vertexCount = 0;
		this->indexCount  = 0;

		this->drawCallsCount++;
		
	}

	void Renderer3D::performShadowMapDrawCalls() {
		this->bindUniformBuffers();
		this->vertexArray.bind();

		// upload the data to vertex/index buffer in the gpu
		this->vertexBuffer.pushData(this->verticesBegin, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer.pushData(this->indices, sizeof(unsigned int) * this->indexCount);

		// update directional lights depth buffers
		depthshaderProgram.bind();
		for (auto& directionalLight : this->directionalLights) {
			CurrentDirectionalLightUniformBufferData lightData{ directionalLight.getViewProjectionMatrix() };
			
			this->currentDirectionalLightUniformBuffer.pushData(&lightData, sizeof(CurrentDirectionalLightUniformBufferData));

			int currentViewPortWidth = DrawApi::getViewPortWidth();
			int currentViewPortHeight = DrawApi::getViewPortHeight();

			DrawApi::setViewPortSize(
				directionalLight.depthMapTexture->getWidth(),
				directionalLight.depthMapTexture->getHeight());

			directionalLight.depthMapFrameBuffer->bind();

			// drawcall
			DrawApi::drawWithIdexes(this->indexCount);
			this->drawCallsCount++;
			// end drawcall

			directionalLight.depthMapFrameBuffer->unbind();

			DrawApi::setViewPortSize(currentViewPortWidth, currentViewPortHeight);
		}
		// end update directional lights depth buffers

		// update point lights depth buffers
		cubeMapDepthMapShaderProgram.bind();

		for (auto& pointLight : this->pointLights) {
			CurrentPointLightUniformBufferData lightData;

			lightData.lightPosition = { pointLight.position, 0.0f };
			lightData.farPlane = pointLight.farPlane;
			std::memcpy(&lightData.shadowMatrices, pointLight.getViewShadowMatrices().data(), sizeof(lightData.shadowMatrices));

			this->currentPointLightUniformBuffer.pushData(&lightData, sizeof(CurrentPointLightUniformBufferData));

			int currentViewPortWidth = DrawApi::getViewPortWidth();
			int currentViewPortHeight = DrawApi::getViewPortHeight();

			DrawApi::setViewPortSize(
				pointLight.depthMapCubeMap->getWidth(),
				pointLight.depthMapCubeMap->getHeight());

			pointLight.depthMapFrameBuffer->bind();

			// drawcall
			DrawApi::drawWithIdexes(this->indexCount);
			this->drawCallsCount++;
			// end drawcall

			pointLight.depthMapFrameBuffer->unbind();

			DrawApi::setViewPortSize(currentViewPortWidth, currentViewPortHeight);
		}

		// end update point lights depth buffers

		this->vertices = this->verticesBegin;
		this->vertexCount = 0;
		this->indexCount  = 0;

	}

}