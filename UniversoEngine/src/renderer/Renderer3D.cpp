#include "Renderer3D.h"

#include <algorithm>
#include <cstring>
#include <string>

#include "../debug/Assert.h"

namespace engine {

	void addDefaultVertexAttributePointers(VertexBuffer& vertexBuffer) {
		vertexBuffer.addAttributePointer(AttriuteType::Vec3,  offsetof(Vertex, position));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3,  offsetof(Vertex, normal));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3,  offsetof(Vertex, ambient));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3,  offsetof(Vertex, diffuse));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3,  offsetof(Vertex, specular));
		vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, shininess));
		vertexBuffer.addAttributePointer(AttriuteType::Vec2,  offsetof(Vertex, textureCoords));
		vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, textureSlotIndex));
	}

	Vertex transformVertex(const Vertex& vertex, const Renderer3D::MeshData& meshData) {
		Texture* texture = meshData.material->getTexture();

		Vertex transformedVertex;
		transformedVertex.position = meshData.transform * glm::vec4(vertex.position, 1.0f);
		transformedVertex.normal = glm::mat3(glm::transpose(glm::inverse(meshData.transform))) * vertex.normal;
		transformedVertex.ambient = meshData.material->ambient;
		transformedVertex.diffuse = meshData.material->diffuse;
		transformedVertex.specular = meshData.material->specular;
		transformedVertex.shininess = meshData.material->shininess;
		transformedVertex.textureCoords = vertex.textureCoords;
		transformedVertex.textureSlotIndex = texture != nullptr ? texture->getSlot() : -1.0f;

		return transformedVertex;
	}

	Renderer3D::Renderer3D() {
		dynamicRenderingData.vertices = new Vertex[dynamicRenderingData.maxVertices];
		dynamicRenderingData.verticesBegin = dynamicRenderingData.vertices;
		dynamicRenderingData.indices = new unsigned int[dynamicRenderingData.maxIndices];

		dynamicRenderingData.vertexArray.addVertexBuffer(dynamicRenderingData.vertexBuffer);
		dynamicRenderingData.vertexArray.addIndexBuffer(dynamicRenderingData.indexBuffer);

		addDefaultVertexAttributePointers(dynamicRenderingData.vertexBuffer);

		fragmentShader.addMacroDefinition("MAX_POINT_LIGHTS", std::to_string(PointLight::maxPointLights));
		fragmentShader.addMacroDefinition("MAX_DIRECTIONAL_LIGHTS", std::to_string(DirectionalLight::maxDirectionalLights));
		fragmentShader.addMacroDefinition("MAX_TEXTURES", std::to_string(Texture::maxTextures));
		fragmentShader.addMacroDefinition("MAX_CUBE_MAPS", std::to_string(Texture::maxCubeMaps));

		shaderProgram.attachShader(vertexShader);
		shaderProgram.attachShader(fragmentShader);
		shaderProgram.bind();

		lightingData.depthBufferTextureShaderProgram.attachShader(lightingData.depthBufferTextureVertexShader);
		lightingData.depthBufferTextureShaderProgram.attachShader(lightingData.depthBufferTextureFragmentShader);

		lightingData.depthBufferCubeMapShaderProgram.attachShader(lightingData.depthBufferCubeMapVertexShader);
		lightingData.depthBufferCubeMapShaderProgram.attachShader(lightingData.depthBufferCubeMapGeometryShader);
		lightingData.depthBufferCubeMapShaderProgram.attachShader(lightingData.depthBufferCubeMapFragmentShader);

		// cubemap skybox
		skyBoxData.vertexArray.addVertexBuffer(skyBoxData.vertexBuffer);

		skyBoxData.vertexBuffer.addAttributePointer(AttriuteType::Vec3, 0);

		glm::vec3 skyboxVertices[36];

		Mesh cubeMesh{ "UniversoEngine/resources/meshes/cube.obj", false };
		const std::vector<Vertex>& cubeMeshVertices = cubeMesh.getVertices();

		for (int i = 0; i < cubeMeshVertices.size(); i++)
			skyboxVertices[i] = cubeMeshVertices[i].position;

		skyBoxData.vertexBuffer.pushData(skyboxVertices, sizeof(skyboxVertices));

		skyBoxData.shaderProgram.attachShader(skyBoxData.vertexShader);
		skyBoxData.shaderProgram.attachShader(skyBoxData.fragmentShader);
	}

	Renderer3D::~Renderer3D() {
		delete[] this->dynamicRenderingData.verticesBegin;
		delete[] this->dynamicRenderingData.indices;
	}

	void Renderer3D::beginFrame(Camera& camera) {
		this->drawCallsCount = 0;
		
		updateCameraUniformBuffer(camera);
		
		clearLights();
		clearBoundTextures();
		clearDynamicMeshes();
	}

	void Renderer3D::endFrame() {
		updateLightsUniformBuffers();
		updateLightsDepthBufferTextures();

		drawDynamicMeshes(&this->shaderProgram);
		drawStaticMeshes(&this->shaderProgram);
		drawSkyBox();
	}

	void Renderer3D::addPointLight(PointLight pointLight, glm::mat4 transform) {
		ASSERT(lightingData.pointLights.size() + 1 <= PointLight::maxPointLights, "Maximum point lights exceded");

		pointLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		lightingData.pointLights.push_back(pointLight);

		bindCubeMap(pointLight.getDepthBufferCubeMap());
	}

	void Renderer3D::addDirectionalLight(DirectionalLight directionalLight, glm::mat4 transform) {
		ASSERT(lightingData.directionalLights.size() + 1 <= DirectionalLight::maxDirectionalLights, "Maximum directional lights exceded");

		directionalLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		lightingData.directionalLights.push_back(directionalLight);

		bindTexture(directionalLight.getDepthBufferTexture());
	}

	void Renderer3D::drawDynamicMesh(MeshData meshData) { 
		dynamicRenderingData.meshDataList.push_back(meshData);
	}

	void Renderer3D::drawStaticMesh(MeshData meshData) {
		staticRenderingData.shouldCreateBuffers = true;
		staticRenderingData.meshDataList.push_back(meshData);
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

	void Renderer3D::setSkyBoxCubeMap(CubeMap* skyBoxCubeMap) {
		this->skyBoxData.cubeMap = skyBoxCubeMap;
	}

	void Renderer3D::updateLightsDepthBufferTextures() {

		// update directional lights frame buffers
		for (auto& directionalLight : lightingData.directionalLights) {
			CurrentDirectionalLightUniformBufferData lightData{ directionalLight.getViewProjectionMatrix() };
			lightingData.currentDirectionalLightUniformBuffer.pushData(&lightData, sizeof(CurrentDirectionalLightUniformBufferData));

			auto frameBuffer = directionalLight.getDepthBufferFrameBuffer();

			frameBuffer->bind();
			DrawApi::clearDepthBuffer();
			frameBuffer->unbind();

			int currentViewPortWidth = DrawApi::getViewPortWidth();
			int currentViewPortHeight = DrawApi::getViewPortHeight();

			auto depthBufferTexture = directionalLight.getDepthBufferTexture();
			DrawApi::setViewPortSize(depthBufferTexture->getWidth(), depthBufferTexture->getHeight());

			drawDynamicMeshes(&lightingData.depthBufferTextureShaderProgram, frameBuffer);
			drawStaticMeshes(&lightingData.depthBufferTextureShaderProgram, frameBuffer);

			DrawApi::setViewPortSize(currentViewPortWidth, currentViewPortHeight);
		}

		// update point lights frame buffers
		for (auto& pointLight : lightingData.pointLights) {
			CurrentPointLightUniformBufferData lightData;

			lightData.lightPosition = { pointLight.position, 0.0f };
			lightData.farPlane = pointLight.farPlane;
			std::memcpy(&lightData.shadowMatrices, pointLight.getViewShadowMatrices().data(), sizeof(lightData.shadowMatrices));

			lightingData.currentPointLightUniformBuffer.pushData(&lightData, sizeof(CurrentPointLightUniformBufferData));

			auto frameBuffer = pointLight.getDepthBufferFrameBuffer();

			frameBuffer->bind();
			DrawApi::clearDepthBuffer();
			frameBuffer->unbind();

			int currentViewPortWidth = DrawApi::getViewPortWidth();
			int currentViewPortHeight = DrawApi::getViewPortHeight();

			auto depthBufferCubeMap = pointLight.getDepthBufferCubeMap();
			DrawApi::setViewPortSize(depthBufferCubeMap->getWidth(), depthBufferCubeMap->getHeight());

			drawDynamicMeshes(&lightingData.depthBufferCubeMapShaderProgram, frameBuffer);
			drawStaticMeshes(&lightingData.depthBufferCubeMapShaderProgram, frameBuffer);

			DrawApi::setViewPortSize(currentViewPortWidth, currentViewPortHeight);
		}
	}

	void Renderer3D::drawStaticMeshes(ShaderProgram* targetShaderProgram, FrameBuffer* frameBufferTarget) {
		bindUniformBuffers();

		targetShaderProgram->bind();

		staticRenderingData.vertexArray.bind();

		if (frameBufferTarget != nullptr) {
			frameBufferTarget->bind();
		}
		else {
			FrameBuffer::bindDefaultFrameBuffer();
		}

		if (staticRenderingData.shouldCreateBuffers) {

			staticRenderingData.shouldCreateBuffers = false;

			staticRenderingData.vertexCount = 0;
			staticRenderingData.indexCount = 0;

			staticRenderingData.vertices.clear();
			staticRenderingData.indices.clear();

			for (auto& meshData : staticRenderingData.meshDataList) {

				bindTexture(meshData.material->getTexture());

				for (const Vertex& vertex : meshData.mesh->getVertices()) {
					Vertex transformedVertex = transformVertex(vertex, meshData);
					staticRenderingData.vertices.push_back(transformedVertex);
				}

				int indexOffset = staticRenderingData.vertexCount;
				for (int index : meshData.mesh->getIndices()) {
					staticRenderingData.indices.push_back(indexOffset + index);
					staticRenderingData.indexCount++;
				}

				staticRenderingData.vertexCount += meshData.mesh->getVertexCount();
			}

			staticRenderingData.vertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex), staticRenderingData.vertexCount);

			addDefaultVertexAttributePointers(*staticRenderingData.vertexBuffer);

			staticRenderingData.indexBuffer = std::make_unique<IndexBuffer>(staticRenderingData.indexCount);

			staticRenderingData.vertexArray.addVertexBuffer(*staticRenderingData.vertexBuffer);
			staticRenderingData.vertexArray.addIndexBuffer(*staticRenderingData.indexBuffer);

			staticRenderingData.vertexBuffer->pushData(staticRenderingData.vertices.data(), sizeof(Vertex) * staticRenderingData.vertexCount);
			staticRenderingData.indexBuffer->pushData(staticRenderingData.indices.data(), sizeof(unsigned int) * staticRenderingData.indexCount);
		}

		DrawApi::drawWithIdexes(staticRenderingData.indexCount);
	}

	void Renderer3D::drawDynamicMeshes(ShaderProgram* targetShaderProgram, FrameBuffer* frameBufferTarget) {

		this->bindUniformBuffers();

		targetShaderProgram->bind();

		if (frameBufferTarget != nullptr) {
			frameBufferTarget->bind();
		}
		else {
			FrameBuffer::bindDefaultFrameBuffer();
		}


		for (auto& meshData : dynamicRenderingData.meshDataList) {
			ASSERT(meshData.mesh->getVertexCount() <= dynamicRenderingData.maxVertices, "Mesh does not fit in the drawcall");

			bool batchIsFull = meshData.mesh->getVertexCount() + dynamicRenderingData.vertexCount > dynamicRenderingData.maxVertices;

			if (batchIsFull) {
				this->flushDynamicRenderingBatch();
			}

			bindTexture(meshData.material->getTexture());

			for (const Vertex& vertex : meshData.mesh->getVertices()) {
				Vertex transformedVertex = transformVertex(vertex, meshData);
				std::memcpy(dynamicRenderingData.vertices, &transformedVertex, sizeof(Vertex));
				dynamicRenderingData.vertices++;
			}

			int indexOffset = dynamicRenderingData.vertexCount;
			for (int index : meshData.mesh->getIndices()) {
				dynamicRenderingData.indices[dynamicRenderingData.indexCount] = indexOffset + index;
				dynamicRenderingData.indexCount++;
			}

			dynamicRenderingData.vertexCount += meshData.mesh->getVertexCount();
		}

		this->flushDynamicRenderingBatch();
	}

	void Renderer3D::drawSkyBox() {
		// draw the sky box
		if (this->skyBoxData.cubeMap != nullptr) {
			DrawApi::setDepthFunctionToLessOrEqual();
			DrawApi::enableDepthMask(false);
			skyBoxData.shaderProgram.bind();
			skyBoxData.vertexArray.bind();
			skyBoxData.cubeMap->bind(0);
			DrawApi::draw(36);
			this->drawCallsCount++;
			DrawApi::enableDepthMask(true);
			DrawApi::setDepthFunctionToLess();
		}
	}

	void Renderer3D::flushDynamicRenderingBatch() {
		dynamicRenderingData.vertexArray.bind();

		// upload the data to vertex/index buffer in the gpu
		dynamicRenderingData.vertexBuffer.pushData(dynamicRenderingData.verticesBegin, sizeof(Vertex) * dynamicRenderingData.vertexCount);
		dynamicRenderingData.indexBuffer.pushData(dynamicRenderingData.indices, sizeof(unsigned int) * dynamicRenderingData.indexCount);

		DrawApi::drawWithIdexes(dynamicRenderingData.indexCount);

		dynamicRenderingData.vertices = dynamicRenderingData.verticesBegin;
		dynamicRenderingData.vertexCount = 0;
		dynamicRenderingData.indexCount = 0;

		this->drawCallsCount++;

	}

	void Renderer3D::bindUniformBuffers() {
		cameraUniformBuffer.bind(0);
		lightingData.lightsUniformBuffer.bind(1);
		lightingData.currentPointLightUniformBuffer.bind(2);
		lightingData.currentDirectionalLightUniformBuffer.bind(3);
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

	void Renderer3D::updateCameraUniformBuffer(Camera& camera) {

		int currentViewPortWidth = DrawApi::getViewPortWidth();
		int currentViewPortHeight = DrawApi::getViewPortHeight();

		CameraUniformBufferData cameraUniformBufferData;

		cameraUniformBufferData.viewProjectionMatrix = camera.getViewProjectionMatrix(currentViewPortWidth, currentViewPortHeight);
		cameraUniformBufferData.viewMatrix = camera.getViewMatrix();
		cameraUniformBufferData.projectionMatrix = camera.getProjectionMatrix(currentViewPortWidth, currentViewPortHeight);
		cameraUniformBufferData.position = camera.position;

		cameraUniformBuffer.pushData(&cameraUniformBufferData, sizeof(CameraUniformBufferData));
	}

	void Renderer3D::updateLightsUniformBuffers() {
		// update lights uniform buffer
		LightsUniformBufferData lightsUniformBufferData;

		lightsUniformBufferData.numberOfPointLights = lightingData.pointLights.size();
		lightsUniformBufferData.numberOfDirectionalLights = lightingData.directionalLights.size();

		for (int i = 0; i < lightingData.pointLights.size(); i++) {
			PointLightUniformBufferData pointLightData;
			pointLightData.position = { lightingData.pointLights[i].position, 0.0f };
			pointLightData.ambient = { lightingData.pointLights[i].ambient, 0.0f };
			pointLightData.diffuse = { lightingData.pointLights[i].diffuse, 0.0f };
			pointLightData.specular = { lightingData.pointLights[i].specular, 0.0f };
			pointLightData.constant = lightingData.pointLights[i].constant;
			pointLightData.linear = lightingData.pointLights[i].linear;
			pointLightData.quadratic = lightingData.pointLights[i].quadratic;
			pointLightData.farPlane = lightingData.pointLights[i].farPlane;
			pointLightData.cubeMapSlotIndex = lightingData.pointLights[i].getDepthBufferCubeMap()->getSlot() - Texture::maxTextures;

			lightsUniformBufferData.pointLights[i] = pointLightData;
		}

		for (int i = 0; i < lightingData.directionalLights.size(); i++) {
			DirectionalLightUniformBufferData directionalLightData;
			directionalLightData.position = { lightingData.directionalLights[i].position, 0.0f };
			directionalLightData.ambient = { lightingData.directionalLights[i].ambient, 0.0f };
			directionalLightData.diffuse = { lightingData.directionalLights[i].diffuse, 0.0f };
			directionalLightData.specular = { lightingData.directionalLights[i].specular, 0.0f };
			directionalLightData.textureSlotIndex = lightingData.directionalLights[i].getDepthBufferTexture()->getSlot();
			directionalLightData.viewProjection = lightingData.directionalLights[i].getViewProjectionMatrix();

			lightsUniformBufferData.directionalLights[i] = directionalLightData;
		}

		lightingData.lightsUniformBuffer.pushData(&lightsUniformBufferData, sizeof(LightsUniformBufferData));
	}

	void Renderer3D::clearLights() {
		// clear the lights vectors
		lightingData.pointLights.clear();
		lightingData.directionalLights.clear();
	}

	void Renderer3D::clearBoundTextures() {
		this->boundTextures.clear();
		this->currentTextureSlot = 0;

		this->boundCubeMaps.clear();
		this->currentCubeMapSlot = 0;
	}

	void Renderer3D::clearDynamicMeshes() {
		dynamicRenderingData.meshDataList.clear();
	}

}