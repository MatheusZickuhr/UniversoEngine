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

		fragmentShader.addMacroDefinition("MAX_POINT_LIGHTS", std::to_string(PointLight::maxPointLights));
		fragmentShader.addMacroDefinition("MAX_DIRECTIONAL_LIGHTS", std::to_string(DirectionalLight::maxDirectionalLights));
		fragmentShader.addMacroDefinition("MAX_TEXTURES", std::to_string(Texture::maxTextures));
		fragmentShader.addMacroDefinition("MAX_CUBE_MAPS", std::to_string(Texture::maxCubeMaps));

		shaderProgram.attachShader(vertexShader);
		shaderProgram.attachShader(fragmentShader);
		shaderProgram.bind();

		depthTextureShaderProgram.attachShader(depthTextureVertexShader);
		depthTextureShaderProgram.attachShader(depthTextureFragmentShader);

		depthCubeMapShaderProgram.attachShader(depthCubeMapVertexShader);
		depthCubeMapShaderProgram.attachShader(depthCubeMapGeometryShader);
		depthCubeMapShaderProgram.attachShader(depthCubeMapFragmentShader);

		// cubemap skybox
		skyBoxVertexArray.addVertexBuffer(skyBoxVertexBuffer);

		skyBoxVertexBuffer.addAttributePointer(AttriuteType::Vec3, 0);

		glm::vec3 skyboxVertices[36];

		Mesh cubeMesh{ "UniversoEngine/resources/meshes/cube.obj", false };
		const std::vector<Vertex>& cubeMeshVertices = cubeMesh.getVertices();

		for (int i = 0; i < cubeMeshVertices.size(); i++)
			skyboxVertices[i] = cubeMeshVertices[i].position;

		skyBoxVertexBuffer.pushData(skyboxVertices, sizeof(skyboxVertices));

		skyBoxShaderProgram.attachShader(skyBoxVertexShader);
		skyBoxShaderProgram.attachShader(skyBoxFragmentShader);
	}

	Renderer3D::~Renderer3D() {
		delete[] this->verticesBegin;
		delete[] this->indices;
	}

	void Renderer3D::beginFrame(Camera& camera) {
		this->drawCallsCount = 0;

		// update camera uniform buffer
		int currentViewPortWidth = DrawApi::getViewPortWidth();
		int currentViewPortHeight = DrawApi::getViewPortHeight();

		CameraUniformBufferData cameraUniformBufferData{
			camera.getViewProjectionMatrix(currentViewPortWidth, currentViewPortHeight),
			camera.getViewMatrix(),
			camera.getProjectionMatrix(currentViewPortWidth, currentViewPortHeight),
			camera.position
		};

		cameraUniformBuffer.pushData(&cameraUniformBufferData, sizeof(CameraUniformBufferData));

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

	void Renderer3D::endFrame() {
		// update lights uniform buffer
		LightsUniformBufferData lightsUniformBufferData;

		lightsUniformBufferData.numberOfPointLights = pointLights.size();
		lightsUniformBufferData.numberOfDirectionalLights = directionalLights.size();

		for (int i = 0; i < this->pointLights.size(); i++)
			lightsUniformBufferData.pointLights[i] = this->pointLights[i].getData();

		for (int i = 0; i < this->directionalLights.size(); i++)
			lightsUniformBufferData.directionalLights[i] = this->directionalLights[i].getData();

		this->lightsUniformBuffer.pushData(&lightsUniformBufferData, sizeof(LightsUniformBufferData));

		// render the scene
		shadowPass();
		renderPass();

		frameDrawData.clear();

		this->boundTextures.clear();
		this->currentTextureSlot = 0;

		this->boundCubeMaps.clear();
		this->currentCubeMapSlot = 0;

		// draw the sky box
		if (this->skyBoxCubeMap != nullptr) {
			DrawApi::setDepthFunctionToLessOrEqual();
			DrawApi::enableDepthMask(false);
			skyBoxShaderProgram.bind();
			skyBoxVertexArray.bind();
			skyBoxCubeMap->bind(0);
			DrawApi::draw(36);
			this->drawCallsCount++;
			DrawApi::enableDepthMask(true);
			DrawApi::setDepthFunctionToLess();
		}
	}

	void Renderer3D::addPointLight(PointLight pointLight, glm::mat4 transform) {
		ASSERT(this->pointLights.size() + 1 <= PointLight::maxPointLights, "Maximum point lights exceded");

		pointLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->pointLights.push_back(pointLight);

		bindCubeMap(pointLight.depthMapCubeMap.get());
	}

	void Renderer3D::addDirectionalLight(DirectionalLight directionalLight, Camera& camera, glm::mat4 transform) {
		ASSERT(this->directionalLights.size() + 1 <= DirectionalLight::maxDirectionalLights, "Maximum directional lights exceded");

		glm::vec3 lightPosition = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		auto lightDirection = glm::vec3(0.0f, 0.0f, 0.0f) - lightPosition;

		directionalLight.position = camera.front - lightDirection;
		directionalLight.lookAt = camera.front;

		this->directionalLights.push_back(directionalLight);

		bindTexture(directionalLight.depthMapTexture.get());
	}

	void Renderer3D::renderPass() {

		for (auto& drawData : frameDrawData) {
			ASSERT(drawData.mesh->getVertexCount() <= maxVertices, "Mesh does not fit in the drawcall");

			if (drawData.mesh->getVertexCount() + this->vertexCount > maxVertices) {
				this->performDrawCall();
			}

			this->bindTexture(drawData.material->getTexture());

			Texture* texture = drawData.material->getTexture();

			for (const Vertex& vertex : drawData.mesh->getVertices()) {
				this->vertices->position = drawData.transform * glm::vec4(vertex.position, 1.0f);
				this->vertices->normal = glm::mat3(glm::transpose(glm::inverse(drawData.transform))) * vertex.normal;
				this->vertices->ambient = drawData.material->ambient;
				this->vertices->diffuse = drawData.material->diffuse;
				this->vertices->specular = drawData.material->specular;
				this->vertices->shininess = drawData.material->shininess;
				this->vertices->textureCoords = vertex.textureCoords;
				this->vertices->textureSlotIndex = texture != nullptr ? texture->getSlot() : -1.0f;
				this->vertices++;
			}

			int indexOffset = this->vertexCount;
			for (int index : drawData.mesh->getIndices()) {
				this->indices[this->indexCount] = indexOffset + index;
				this->indexCount++;
			}

			this->vertexCount += drawData.mesh->getVertexCount();
		}

		this->performDrawCall();
	}

	void Renderer3D::shadowPass() {

		for (auto drawData : frameDrawData) {

			ASSERT(drawData.mesh->getVertexCount() <= maxVertices, "Mesh does not fit in the drawcall");

			if (drawData.mesh->getVertexCount() + this->vertexCount > maxVertices) {
				this->performShadowMapDrawCalls();
			}

			for (const Vertex& vertex : drawData.mesh->getVertices()) {
				this->vertices->position = drawData.transform * glm::vec4(vertex.position, 1.0f);
				this->vertices++;
			}

			int indexOffset = this->vertexCount;
			for (int index : drawData.mesh->getIndices()) {
				this->indices[this->indexCount] = indexOffset + index;
				this->indexCount++;
			}
			this->vertexCount += drawData.mesh->getVertexCount();
		}

		this->performShadowMapDrawCalls();
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

		bool textureBound = std::find(this->boundTextures.begin(), this->boundTextures.end(), texture)
			!= this->boundTextures.end();

		if (!textureBound) {
			texture->bind(this->currentTextureSlot);
			this->boundTextures.push_back(texture);
			this->currentTextureSlot++;
		}
	}

	void Renderer3D::bindCubeMap(Texture* cubeMap) {
		ASSERT(currentCubeMapSlot < Texture::maxCubeMaps, "Maximum cube map slot exceded");

		if (cubeMap == nullptr) return;

		bool cubeMapBound = std::find(this->boundCubeMaps.begin(), this->boundCubeMaps.end(), cubeMap)
			!= this->boundCubeMaps.end();

		if (!cubeMapBound) {
			cubeMap->bind(this->currentCubeMapSlot + Texture::maxTextures);
			this->boundCubeMaps.push_back(cubeMap);
			this->currentCubeMapSlot++;
		}
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
		this->indexCount = 0;

		this->drawCallsCount++;

	}

	void Renderer3D::performShadowMapDrawCalls() {
		this->bindUniformBuffers();
		this->vertexArray.bind();

		// upload the data to vertex/index buffer in the gpu
		this->vertexBuffer.pushData(this->verticesBegin, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer.pushData(this->indices, sizeof(unsigned int) * this->indexCount);

		// update directional lights depth buffers
		depthTextureShaderProgram.bind();
		for (auto& directionalLight : this->directionalLights) {
			CurrentDirectionalLightUniformBufferData 
				lightData{ directionalLight.getViewProjectionMatrix() };

			this->currentDirectionalLightUniformBuffer
				.pushData(&lightData, sizeof(CurrentDirectionalLightUniformBufferData));

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
		depthCubeMapShaderProgram.bind();

		for (auto& pointLight : this->pointLights) {
			CurrentPointLightUniformBufferData lightData;

			lightData.lightPosition = { pointLight.position, 0.0f };
			lightData.farPlane = pointLight.farPlane;
			std::memcpy(
				&lightData.shadowMatrices,
				pointLight.getViewShadowMatrices().data(),
				sizeof(lightData.shadowMatrices));

			this->currentPointLightUniformBuffer.pushData(
				&lightData, sizeof(CurrentPointLightUniformBufferData));

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
		this->indexCount = 0;

	}

}