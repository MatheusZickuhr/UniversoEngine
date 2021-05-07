#include "Renderer3D.h"

#include <algorithm>

#include "../utils/Format.h"
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
		vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, textureSlot));

		fragShader.defineInt("MAX_POINT_LIGHTS", PointLight::maxPointLights);
		fragShader.defineInt("MAX_DIRECTIONAL_LIGHTS", DirectionalLight::maxDirectionalLights);
		fragShader.defineInt("MAX_TEXTURES", Texture::maxTextures);
		fragShader.defineInt("MAX_CUBE_MAPS", Texture::maxCubeMaps);


		shaderProgram.attachShader(vertexShader);
		shaderProgram.attachShader(fragShader);
		shaderProgram.bind();

		// add the textures/cubeMap slots to the shader
		{
			int textureSlots[Texture::maxTextures];
			for (int i = 0; i < Texture::maxTextures; i++) textureSlots[i] = i;
			this->shaderProgram.setIntArrayUniform("textureSlots", Texture::maxTextures, textureSlots);
		}

		{
			int cubeMapSlots[Texture::maxCubeMaps];
			for (int i = 0; i < Texture::maxCubeMaps; i++) cubeMapSlots[i] = i + Texture::maxTextures;
			this->shaderProgram.setIntArrayUniform("cubeMapSlots", Texture::maxCubeMaps, cubeMapSlots);
		}
		
		shaderProgram.setIntUniform("numberOfPointLights", 0);
		shaderProgram.setIntUniform("numberOfDirectionalLights", 0);

		depthshaderProgram.attachShader(depthVertexShader);
		depthshaderProgram.attachShader(depthFragShader);

		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapVertexShader);
		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapGeometryShader);
		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapFragmentShader);
	}

	void Renderer3D::startDrawing(Camera& camera) {
		int currentViewPortWidth = DrawApi::getViewPortWidth();
		int currentViewPortHeight = DrawApi::getViewPortHeight();

		this->shaderProgram.setMat4Uniform("viewProjection",
			camera.getViewProjectionMatrix(currentViewPortWidth, currentViewPortHeight));
		this->shaderProgram.setVec3Uniform("viewPosition", camera.position);
		
	}

	void Renderer3D::endDrawing() {
		this->performDrawCall();
		
		// unbind all the textures/ cubemaps
		this->clearBindedTextures();
		this->clearBindedCubeMaps();
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
			this->vertices->textureSlot = texture != nullptr ? texture->getSlot() : -1.0f;
			this->vertices++;
		}

		for (int i = this->indexCount; i < mesh->getVertexCount() + this->indexCount; i++)
			this->indices[i] = i;

		this->vertexCount += mesh->getVertexCount();
		this->indexCount += mesh->getVertexCount();
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

		for (int i = this->indexCount; i < mesh->getVertexCount() + this->indexCount; i++)
			this->indices[i] = i;

		this->vertexCount += mesh->getVertexCount();
		this->indexCount += mesh->getVertexCount();
	}

	void Renderer3D::drawPointLight(PointLight pointLight, glm::mat4 transform) {
		ASSERT(this->pointLights.size() + 1 <= PointLight::maxPointLights, "Maximum point lights exceded");

		pointLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->pointLights.push_back(pointLight);

		bindCubeMap(pointLight.depthMapCubeMap.get());

		unsigned int lightIndex = this->pointLights.size() - 1;

		shaderProgram.setIntUniform("numberOfPointLights", pointLights.size());

		shaderProgram.setVec3Uniform(format("pointLights[%d].position", lightIndex), pointLight.position);
		shaderProgram.setVec3Uniform(format("pointLights[%d].ambient", lightIndex), pointLight.ambient);
		shaderProgram.setVec3Uniform(format("pointLights[%d].diffuse", lightIndex), pointLight.diffuse);
		shaderProgram.setVec3Uniform(format("pointLights[%d].specular", lightIndex), pointLight.specular);
		shaderProgram.setFloatUniform(format("pointLights[%d].constant", lightIndex), pointLight.constant);
		shaderProgram.setFloatUniform(format("pointLights[%d].linear", lightIndex), pointLight.linear);
		shaderProgram.setFloatUniform(format("pointLights[%d].quadratic", lightIndex), pointLight.quadratic);
		shaderProgram.setFloatUniform(format("pointLights[%d].farPlane", lightIndex), pointLight.farPlane);
		shaderProgram.setIntUniform(format("pointLights[%d].cubeMapSlotIndex", lightIndex),
			pointLight.depthMapCubeMap->getSlot() - Texture::maxTextures);
	}

	void Renderer3D::drawDirectionalLight(DirectionalLight directionalLight, glm::mat4 transform) {
		ASSERT(this->directionalLights.size() + 1 <= DirectionalLight::maxDirectionalLights, "Maximum directional lights exceded");

		directionalLight.position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->directionalLights.push_back(directionalLight);

		bindTexture(directionalLight.depthMapTexture.get());

		shaderProgram.setIntUniform("numberOfDirectionalLights", directionalLights.size());

		unsigned int lightIndex = this->directionalLights.size() - 1;

		shaderProgram.setVec3Uniform(format("directionalLights[%d].position", lightIndex), directionalLight.position);
		shaderProgram.setVec3Uniform(format("directionalLights[%d].ambient", lightIndex), directionalLight.ambient);
		shaderProgram.setVec3Uniform(format("directionalLights[%d].diffuse", lightIndex), directionalLight.diffuse);
		shaderProgram.setVec3Uniform(format("directionalLights[%d].specular", lightIndex), directionalLight.specular);
		shaderProgram.setMat4Uniform(format("directionalLights[%d].viewProjection", lightIndex), directionalLight.getViewProjectionMatrix());
		shaderProgram.setIntUniform(format("directionalLights[%d].textureSlotIndex", lightIndex), directionalLight.depthMapTexture->getSlot());
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
		this->vertexArray.bind();

		// upload the data to vertex/index buffer in the gpu
		this->vertexBuffer.pushData(this->verticesBegin, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer.pushData(this->indices, sizeof(unsigned int) * this->indexCount);

		// update directional lights depth buffers
		depthshaderProgram.bind();
		for (int i = 0; i < directionalLights.size(); i++) {
			auto& directionalLight = directionalLights[i];

			depthshaderProgram.setMat4Uniform("lightSpaceMatrix",
				directionalLight.getViewProjectionMatrix());

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

		for (int i = 0; i < pointLights.size(); i++) {
			auto& pointLight = pointLights[i];

			cubeMapDepthMapShaderProgram.setVec3Uniform("lightPos", pointLight.position);
			cubeMapDepthMapShaderProgram.setFloatUniform("far_plane", pointLight.farPlane);

			auto pointLightProjectionMatrices = pointLight.getViewProjectionMatrices();
			for (int j = 0; j < pointLightProjectionMatrices.size(); j++) {
				cubeMapDepthMapShaderProgram.setMat4Uniform(
					format("shadowMatrices[%d]", j),
					pointLightProjectionMatrices[j]);
			}

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