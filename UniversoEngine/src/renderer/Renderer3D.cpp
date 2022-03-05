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

	Renderer3D::Renderer3D() {
		drawCallsCount = 0;

		lightingData.currentTextureSlot = lightingData.initialTextureSlot;
		lightingData.currentCubeMapSlot = lightingData.initialCubeMapSlot;

		dynamicRenderingData.vertices = new Vertex[dynamicRenderingData.maxVertices];
		dynamicRenderingData.verticesBegin = dynamicRenderingData.vertices;
		dynamicRenderingData.indices = new unsigned int[dynamicRenderingData.maxIndices];

		dynamicRenderingData.vertexArray.addVertexBuffer(dynamicRenderingData.vertexBuffer);
		dynamicRenderingData.vertexArray.addIndexBuffer(dynamicRenderingData.indexBuffer);

		addDefaultVertexAttributePointers(dynamicRenderingData.vertexBuffer);

		fragmentShader.addMacroDefinition("MAX_POINT_LIGHTS", std::to_string(PointLight::maxPointLights));
		fragmentShader.addMacroDefinition("MAX_DIRECTIONAL_LIGHTS", std::to_string(DirectionalLight::maxDirectionalLights));
		fragmentShader.addMacroDefinition("MAX_TEXTURES", std::to_string(Texture::MAX_TEXTURES));
		fragmentShader.addMacroDefinition("MAX_CUBE_MAPS", std::to_string(Texture::MAX_CUBEMAPS));

		shaderProgram.attachShader(vertexShader);
		shaderProgram.attachShader(fragmentShader);

		lightingData.depthBufferTextureShaderProgram.attachShader(lightingData.depthBufferTextureVertexShader);
		lightingData.depthBufferTextureShaderProgram.attachShader(lightingData.depthBufferTextureFragmentShader);

		lightingData.depthBufferCubeMapShaderProgram.attachShader(lightingData.depthBufferCubeMapVertexShader);
		lightingData.depthBufferCubeMapShaderProgram.attachShader(lightingData.depthBufferCubeMapGeometryShader);
		lightingData.depthBufferCubeMapShaderProgram.attachShader(lightingData.depthBufferCubeMapFragmentShader);

		// cubemap skybox
		skyBoxData.vertexArray.addVertexBuffer(skyBoxData.vertexBuffer);

		skyBoxData.vertexBuffer.addAttributePointer(AttriuteType::Vec3, 0);

		glm::vec3 skyboxVertices[36];

		Mesh cubeMesh{ std::string(ENGINE_ASSET_DIRECTORY) + "meshes/cube.obj", false };
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
		clearLightingBoundTextures();
		clearDynamicMeshes();
	}

	void Renderer3D::endFrame() {
		updateLightsUniformBuffers();
		updateLightsDepthBufferTextures();

		drawDynamicMeshes(&this->shaderProgram);
		drawStaticMeshes(&this->shaderProgram);
		drawSkyBox();
	}

	void Renderer3D::drawPointLight(const PointLight& pointLight) {
		ASSERT(lightingData.pointLights.size() + 1 <= PointLight::maxPointLights, "Maximum point lights exceded");
		lightingData.pointLights.push_back(pointLight);
		bindLightingCubeMap(pointLight.getDepthBufferCubeMap());
	}

	void Renderer3D::drawDirectionalLight(const DirectionalLight& directionalLight) {
		ASSERT(lightingData.directionalLights.size() + 1 <= DirectionalLight::maxDirectionalLights, "Maximum directional lights exceded");
		lightingData.directionalLights.push_back(directionalLight);
		bindLightingTexture(directionalLight.getDepthBufferTexture());
	}

	void Renderer3D::drawDynamicMesh(Mesh* mesh, Material* material, const glm::mat4& transform, const unsigned int& renderId) {
		
		dynamicRenderingData.meshDataList.push_back({
			.mesh = mesh,
			.material = material,
			.transform = transform,
			.renderId = renderId});
	}

	void Renderer3D::drawStaticMesh(Mesh* mesh, Material* material, const glm::mat4& transform, const unsigned int& renderId) {
		StaticMeshData meshData { 
			.mesh = mesh,
			.material = material,
			.transform = transform,
			.renderId = renderId };
	
		for (const Vertex& vertex : meshData.mesh->getVertices()) {
			Vertex transformedVertex = transformAndApplyMateriaToVertex(vertex, meshData.transform, meshData.material);
			meshData.vertices.push_back(transformedVertex);
		}

		for (int index : meshData.mesh->getIndices()) {
			meshData.indices.push_back(index);
		}
		
		meshData.vertexArray = std::make_shared<VertexArray>();
		meshData.vertexBuffer = std::make_shared<VertexBuffer>(sizeof(Vertex), meshData.vertices.size());

		addDefaultVertexAttributePointers(*meshData.vertexBuffer);

		meshData.indexBuffer = std::make_shared<IndexBuffer>(meshData.indices.size());

		meshData.vertexArray->addVertexBuffer(*meshData.vertexBuffer);
		meshData.vertexArray->addIndexBuffer(*meshData.indexBuffer);

		meshData.vertexBuffer->pushData(meshData.vertices.data(), sizeof(Vertex) * meshData.vertices.size());
		meshData.indexBuffer->pushData(meshData.indices.data(), sizeof(unsigned int) * meshData.indices.size());
		
		staticRenderingData.meshDataList.push_back(meshData);
	}

	void Renderer3D::destroyStaticMesh(const unsigned int& renderId) {
		int meshDataIndex = 0;
		bool meshDataFound = false;

		for (auto& meshData: this->staticRenderingData.meshDataList) {
			if (renderId == meshData.renderId) {
				meshDataFound = true;
				break;
			}
			meshDataIndex++;
		}

		if (meshDataFound) {
			this->staticRenderingData.meshDataList.erase(this->staticRenderingData.meshDataList.begin() + meshDataIndex);
		}
	}

	void Renderer3D::clearColor(float r, float g, float b, float a) { 
		DrawApi::clearColor(r, g, b, a);
	}

	unsigned int Renderer3D::getDrawCallsCount() { 
		return this->drawCallsCount;
	}

	void Renderer3D::setSkyBoxCubeMap(CubeMap* skyBoxCubeMap) {
		this->skyBoxData.cubeMap = skyBoxCubeMap;
	}

	void Renderer3D::updateLightsDepthBufferTextures() {

		// update directional lights frame buffers
		for (DirectionalLight& directionalLight : lightingData.directionalLights) {

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

		if (frameBufferTarget != nullptr) {
			frameBufferTarget->bind();
		}
		else {
			FrameBuffer::bindDefaultFrameBuffer();
		}

		
		for (StaticMeshData& meshData : staticRenderingData.meshDataList) {
			if (meshData.material->getTexture() != nullptr) {
				meshData.material->getTexture()->bind(TEXTURE_SLOT);
			}
			meshData.vertexArray->bind();
			DrawApi::drawWithIdexes(meshData.indices.size());
		}

	}

	void Renderer3D::drawDynamicMeshes(ShaderProgram* targetShaderProgram, FrameBuffer* frameBufferTarget) {

		this->bindUniformBuffers();

		targetShaderProgram->bind();

		dynamicRenderingData.vertexArray.bind();

		if (frameBufferTarget != nullptr) {
			frameBufferTarget->bind();
		}
		else {
			FrameBuffer::bindDefaultFrameBuffer();
		}


		for (auto& meshData : dynamicRenderingData.meshDataList) {
			ASSERT(meshData.mesh->getVertexCount() <= dynamicRenderingData.maxVertices, "Mesh does not fit in the drawcall");
			
			Texture* texture = meshData.material->getTexture();
			
			if (texture != nullptr) {
				texture->bind(TEXTURE_SLOT);
			}

			for (const Vertex& vertex : meshData.mesh->getVertices()) {

				Vertex transformedVertex = transformAndApplyMateriaToVertex(vertex, meshData.transform, meshData.material);

				std::memcpy(dynamicRenderingData.vertices, &transformedVertex, sizeof(Vertex));
				dynamicRenderingData.vertices++;
			}

			for (int index : meshData.mesh->getIndices()) {
				dynamicRenderingData.indices[dynamicRenderingData.indexCount] = index;
				dynamicRenderingData.indexCount++;
			}

			dynamicRenderingData.vertexCount += meshData.mesh->getVertexCount();
			
			// upload the data to vertex/index buffer in the gpu
			dynamicRenderingData.vertexBuffer.pushData(dynamicRenderingData.verticesBegin, sizeof(Vertex) * dynamicRenderingData.vertexCount);
			dynamicRenderingData.indexBuffer.pushData(dynamicRenderingData.indices, sizeof(unsigned int) * dynamicRenderingData.indexCount);

			DrawApi::drawWithIdexes(dynamicRenderingData.indexCount);

			dynamicRenderingData.vertices = dynamicRenderingData.verticesBegin;
			dynamicRenderingData.vertexCount = 0;
			dynamicRenderingData.indexCount = 0;

			this->drawCallsCount++;
		}

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

	Vertex Renderer3D::transformAndApplyMateriaToVertex(const Vertex& vertex, glm::mat4& transform, Material* material) {
	
		Texture* texture = material->getTexture();

		Vertex transformedVertex;
		transformedVertex.position =transform * glm::vec4(vertex.position, 1.0f);
		transformedVertex.normal = glm::mat3(glm::transpose(glm::inverse(transform))) * vertex.normal;
		transformedVertex.ambient = material->ambient;
		transformedVertex.diffuse =material->diffuse;
		transformedVertex.specular = material->specular;
		transformedVertex.shininess = material->shininess;
		transformedVertex.textureCoords = vertex.textureCoords;
		transformedVertex.textureSlotIndex = material->getTexture() != nullptr ? TEXTURE_SLOT : NO_TEXTURE_SLOT;

		return transformedVertex;
	}

	void Renderer3D::bindUniformBuffers() {
		cameraUniformBuffer.bind(0);
		lightingData.lightsUniformBuffer.bind(1);
		lightingData.currentPointLightUniformBuffer.bind(2);
		lightingData.currentDirectionalLightUniformBuffer.bind(3);
	}

	void Renderer3D::bindLightingTexture(Texture* texture) {
		ASSERT(lightingData.currentTextureSlot < Texture::MAX_TEXTURES, "Maximum texture slot exceded");

		if (texture == nullptr) return;

		bool textureBound = std::find(lightingData.boundTextures.begin(), lightingData.boundTextures.end(), texture)
			!= lightingData.boundTextures.end();

		if (!textureBound) {
			texture->bind(lightingData.currentTextureSlot);
			lightingData.boundTextures.push_back(texture);
			lightingData.currentTextureSlot++;
		}
	}

	void Renderer3D::bindLightingCubeMap(Texture* cubeMap) {
		ASSERT(lightingData.currentCubeMapSlot < Texture::MAX_CUBEMAPS, "Maximum cube map slot exceded");

		if (cubeMap == nullptr) return;

		bool cubeMapBound = std::find(lightingData.boundCubeMaps.begin(), lightingData.boundCubeMaps.end(), cubeMap)
			!= lightingData.boundCubeMaps.end();

		if (!cubeMapBound) {
			cubeMap->bind(lightingData.currentCubeMapSlot + Texture::MAX_TEXTURES);
			lightingData.boundCubeMaps.push_back(cubeMap);
			lightingData.currentCubeMapSlot++;
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
			pointLightData.cubeMapSlotIndex = lightingData.pointLights[i].getDepthBufferCubeMap()->getSlot() - Texture::MAX_TEXTURES;

			lightsUniformBufferData.pointLights[i] = pointLightData;
		}

		for (int i = 0; i < lightingData.directionalLights.size(); i++) {
			DirectionalLight& directionalLight = lightingData.directionalLights[i];

			DirectionalLightUniformBufferData directionalLightData;
			directionalLightData.position = {directionalLight.position, 0.0f};
			directionalLightData.ambient = { directionalLight.ambient, 0.0f };
			directionalLightData.diffuse = { directionalLight.diffuse, 0.0f };
			directionalLightData.specular = { directionalLight.specular, 0.0f };
			directionalLightData.textureSlotIndex = directionalLight.getDepthBufferTexture()->getSlot();
			directionalLightData.viewProjection = directionalLight.getViewProjectionMatrix();

			lightsUniformBufferData.directionalLights[i] = directionalLightData;
		}

		lightingData.lightsUniformBuffer.pushData(&lightsUniformBufferData, sizeof(LightsUniformBufferData));
	}

	void Renderer3D::clearLights() {
		// clear the lights vectors
		lightingData.pointLights.clear();
		lightingData.directionalLights.clear();
	}

	void Renderer3D::clearLightingBoundTextures() {
		lightingData.boundTextures.clear();
		lightingData.currentTextureSlot = lightingData.initialTextureSlot;

		lightingData.boundCubeMaps.clear();
		lightingData.currentCubeMapSlot = lightingData.initialCubeMapSlot;
	}

	void Renderer3D::clearDynamicMeshes() {
		dynamicRenderingData.meshDataList.clear();
	}

}