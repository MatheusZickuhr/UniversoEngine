#include "Renderer3D.h"

#include <algorithm>
#include <cstring>
#include <string>

#include "../debug/Assert.h"

namespace engine {

	Renderer3D::Renderer3D() {
		dynamicRenderingData.vertices = new Vertex[dynamicRenderingData.maxVertices];
		dynamicRenderingData.verticesBegin = dynamicRenderingData.vertices;
		dynamicRenderingData.indices = new unsigned int[dynamicRenderingData.maxIndices];

		dynamicRenderingData.vertexArray.addVertexBuffer(dynamicRenderingData.vertexBuffer);
		dynamicRenderingData.vertexArray.addIndexBuffer(dynamicRenderingData.indexBuffer);

		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, position));
		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, normal));
		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, ambient));
		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, diffuse));
		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, specular));
		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, shininess));
		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Vec2, offsetof(Vertex, textureCoords));
		dynamicRenderingData.vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, textureSlotIndex));

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

			if (staticRenderingData.vertexBuffer != nullptr) {
				delete staticRenderingData.vertexBuffer;
			}

			if (staticRenderingData.indexBuffer != nullptr) {
				delete staticRenderingData.indexBuffer;
			}

			staticRenderingData.vertexCount = 0;
			staticRenderingData.indexCount  = 0;

			staticRenderingData.vertices.clear();
			staticRenderingData.indices.clear();

			for (auto& meshData : staticRenderingData.meshDataList) {

				this->bindTexture(meshData.material->getTexture());

				Texture* texture = meshData.material->getTexture();

				for (const Vertex& vertex : meshData.mesh->getVertices()) {
					Vertex transformedVertex;

					transformedVertex.position = meshData.transform * glm::vec4(vertex.position, 1.0f);
					transformedVertex.normal = glm::mat3(glm::transpose(glm::inverse(meshData.transform))) * vertex.normal;
					transformedVertex.ambient = meshData.material->ambient;
					transformedVertex.diffuse = meshData.material->diffuse;
					transformedVertex.specular = meshData.material->specular;
					transformedVertex.shininess = meshData.material->shininess;
					transformedVertex.textureCoords = vertex.textureCoords;
					transformedVertex.textureSlotIndex = texture != nullptr ? texture->getSlot() : -1.0f;
					
					staticRenderingData.vertices.push_back(transformedVertex);
				}

				int indexOffset = staticRenderingData.vertexCount;
				for (int index : meshData.mesh->getIndices()) {
					staticRenderingData.indices.push_back(indexOffset + index);
					staticRenderingData.indexCount++;
				}

				staticRenderingData.vertexCount += meshData.mesh->getVertexCount();
			}
			
			staticRenderingData.vertexBuffer = new VertexBuffer{ sizeof(Vertex), staticRenderingData.vertexCount };

			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, position));
			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, normal));
			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, ambient));
			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, diffuse));
			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, specular));
			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Float, offsetof(Vertex, shininess));
			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Vec2, offsetof(Vertex, textureCoords));
			staticRenderingData.vertexBuffer->addAttributePointer(AttriuteType::Float, offsetof(Vertex, textureSlotIndex));


			staticRenderingData.indexBuffer = new IndexBuffer{ staticRenderingData.indexCount };

			staticRenderingData.vertexArray.addVertexBuffer(*staticRenderingData.vertexBuffer);
			staticRenderingData.vertexArray.addIndexBuffer(*staticRenderingData.indexBuffer);

			staticRenderingData.vertexBuffer->pushData(staticRenderingData.vertices.data(), sizeof(Vertex) * staticRenderingData.vertexCount);
			staticRenderingData.indexBuffer->pushData(staticRenderingData.indices.data(), sizeof(unsigned int) * staticRenderingData.indexCount);
		}

		DrawApi::drawWithIdexes(staticRenderingData.indexCount);
	}

	Renderer3D::~Renderer3D() {
		delete[] this->dynamicRenderingData.verticesBegin;
		delete[] this->dynamicRenderingData.indices;
	}

	void Renderer3D::beginFrame(Camera& camera) {
		this->drawCallsCount = 0;
		
		updateCameraUniformBuffer(camera);
		
		clearLightsFrameBuffers();
		clearLights();
	}

	void Renderer3D::endFrame() {
		updateLightsUniformBuffers();

		drawLightsFrameBuffers();
		drawDynamicMeshes(&this->shaderProgram);
		drawStaticMeshes(&this->shaderProgram);
		drawSkyBox();
		
		clearBindedTextures();
		dynamicRenderingData.meshDataList.clear();
	}

	void Renderer3D::addPointLight(PointLight pointLight, glm::mat4 transform) {
		ASSERT(this->pointLights.size() + 1 <= PointLight::maxPointLights, "Maximum point lights exceded");

		pointLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->pointLights.push_back(pointLight);

		bindCubeMap(pointLight.depthMapCubeMap.get());
	}

	void Renderer3D::addDirectionalLight(DirectionalLight directionalLight, glm::mat4 transform) {
		ASSERT(this->directionalLights.size() + 1 <= DirectionalLight::maxDirectionalLights, "Maximum directional lights exceded");

		directionalLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		this->directionalLights.push_back(directionalLight);

		bindTexture(directionalLight.depthMapTexture.get());
	}

	void Renderer3D::drawStaticMesh(MeshData meshData) {
		staticRenderingData.shouldCreateBuffers = true;
		staticRenderingData.meshDataList.push_back(meshData);
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

			this->bindTexture(meshData.material->getTexture());

			Texture* texture = meshData.material->getTexture();

			for (const Vertex& vertex : meshData.mesh->getVertices()) {
				dynamicRenderingData.vertices->position = meshData.transform * glm::vec4(vertex.position, 1.0f);
				dynamicRenderingData.vertices->normal = glm::mat3(glm::transpose(glm::inverse(meshData.transform))) * vertex.normal;
				dynamicRenderingData.vertices->ambient = meshData.material->ambient;
				dynamicRenderingData.vertices->diffuse = meshData.material->diffuse;
				dynamicRenderingData.vertices->specular = meshData.material->specular;
				dynamicRenderingData.vertices->shininess = meshData.material->shininess;
				dynamicRenderingData.vertices->textureCoords = vertex.textureCoords;
				dynamicRenderingData.vertices->textureSlotIndex = texture != nullptr ? texture->getSlot() : -1.0f;
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

	void Renderer3D::drawLightsFrameBuffers() {

		// update directional lights frame buffers
		for (auto& directionalLight : this->directionalLights) {
			CurrentDirectionalLightUniformBufferData lightData{ directionalLight.getViewProjectionMatrix() };
			this->currentDirectionalLightUniformBuffer.pushData(&lightData, sizeof(CurrentDirectionalLightUniformBufferData));

			int currentViewPortWidth = DrawApi::getViewPortWidth();
			int currentViewPortHeight = DrawApi::getViewPortHeight();

			DrawApi::setViewPortSize(directionalLight.depthMapTexture->getWidth(), directionalLight.depthMapTexture->getHeight());
			drawDynamicMeshes(&depthTextureShaderProgram, directionalLight.depthMapFrameBuffer.get());
			drawStaticMeshes(&depthTextureShaderProgram, directionalLight.depthMapFrameBuffer.get());
			DrawApi::setViewPortSize(currentViewPortWidth, currentViewPortHeight);
		}

		// update point lights frame buffers
		for (auto& pointLight : this->pointLights) {
			CurrentPointLightUniformBufferData lightData;

			lightData.lightPosition = { pointLight.position, 0.0f };
			lightData.farPlane = pointLight.farPlane;
			std::memcpy(&lightData.shadowMatrices, pointLight.getViewShadowMatrices().data(), sizeof(lightData.shadowMatrices));

			this->currentPointLightUniformBuffer.pushData(&lightData, sizeof(CurrentPointLightUniformBufferData));

			int currentViewPortWidth = DrawApi::getViewPortWidth();
			int currentViewPortHeight = DrawApi::getViewPortHeight();

			DrawApi::setViewPortSize(pointLight.depthMapCubeMap->getWidth(), pointLight.depthMapCubeMap->getHeight());
			drawDynamicMeshes(&depthCubeMapShaderProgram, pointLight.depthMapFrameBuffer.get());
			drawStaticMeshes(&depthCubeMapShaderProgram, pointLight.depthMapFrameBuffer.get());
			DrawApi::setViewPortSize(currentViewPortWidth, currentViewPortHeight);
		}
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

	void Renderer3D::clearLightsFrameBuffers() {
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
	}

	void Renderer3D::updateLightsUniformBuffers() {
		// update lights uniform buffer
		LightsUniformBufferData lightsUniformBufferData;

		lightsUniformBufferData.numberOfPointLights = pointLights.size();
		lightsUniformBufferData.numberOfDirectionalLights = directionalLights.size();

		for (int i = 0; i < this->pointLights.size(); i++) {
			lightsUniformBufferData.pointLights[i] = this->pointLights[i].getData();
		}

		for (int i = 0; i < this->directionalLights.size(); i++) {
			lightsUniformBufferData.directionalLights[i] = this->directionalLights[i].getData();
		}

		this->lightsUniformBuffer.pushData(&lightsUniformBufferData, sizeof(LightsUniformBufferData));
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

	void Renderer3D::clearLights() {
		// clear the lights vectors
		pointLights.clear();
		directionalLights.clear();
	}

	void Renderer3D::clearBindedTextures() {
		this->boundTextures.clear();
		this->currentTextureSlot = 0;

		this->boundCubeMaps.clear();
		this->currentCubeMapSlot = 0;
	}

}