#include "Renderer3D.h"

#include <utility>

#include "../utils/Format.h"
#include "../debug/Assert.h"

namespace engine {

	Renderer3D::Renderer3D() {

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

		shaderProgram.attachShader(vertexShader.getId());
		shaderProgram.attachShader(fragShader.getId());
		shaderProgram.bind();

		// add the textures slots to the shader
		
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


		// shadow stuff
		
		depthshaderProgram.attachShader(depthVertexShader.getId());
		depthshaderProgram.attachShader(depthFragShader.getId());

		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapVertexShader.getId());
		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapGeometryShader.getId());
		cubeMapDepthMapShaderProgram.attachShader(cubeMapDepthMapFragmentShader.getId());

		
	}

	void Renderer3D::updateDirectionalLightDepthBuffers() {
		depthshaderProgram.bind();

		for (int i = 0; i < directionalLights.size(); i++) {

			auto& directionalLight = directionalLights[i];

			this->bindTexture(directionalLight.depthMapTexture.get());

			depthshaderProgram.setMat4Uniform("lightSpaceMatrix", directionalLight.getViewProjectionMatrix());

			DrawApi::setViewPortSize(directionalLight.depthMapTexture->getWidth(), directionalLight.depthMapTexture->getHeight());

			directionalLight.depthMapFrameBuffer->bind();
			DrawApi::clearDepthBuffer();
			
			DrawApi::cullFrontFace();

			this->render();

			directionalLight.depthMapFrameBuffer->unbind();

			DrawApi::cullBackFace();
		}

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

	void Renderer3D::startDrawing(Camera& camera, const float width, const float height) {

		DrawApi::setViewPortSize(width, height);

		shaderProgram.bind();
		this->shaderProgram.setMat4Uniform("Mvp", camera.getMvp(width, height));
		this->shaderProgram.setVec3Uniform("viewPosition", camera.position);
		this->drawCallsCount = 0;

		this->currentDrawCallBuffer = new DrawCallBuffer{ maxVertices, maxIndices };

		clearDrawCallBuffers();

	}

	void Renderer3D::endDrawing() {
		drawCallBuffers.push_back(currentDrawCallBuffer);
		this->render();
		this->clearBindedTextures();
		this->clearBindedCubeMaps();
	}

	void Renderer3D::drawMesh(Mesh* mesh, Material* material, glm::mat4 transform) {
		
		if (!currentDrawCallBuffer->doesFit(mesh)) {
			
			drawCallBuffers.push_back(currentDrawCallBuffer);
			this->currentDrawCallBuffer = new DrawCallBuffer{ maxVertices, maxIndices };
		}
		
		this->bindTexture(material->getTexture());

		currentDrawCallBuffer->addMesh(mesh, material, transform);
	}

	void Renderer3D::startLightsDrawing() {
		pointLights.clear();
		directionalLights.clear();
	}

	void Renderer3D::endLightsDrawing() {
		this->updatePointLightsDepthBuffers();
		this->updateDirectionalLightDepthBuffers();
		this->updatePointLightsUniforms();
		this->updateDirectionalLightsUniforms();
	}

	void Renderer3D::clearColor(float r, float g, float b, float a) {
		DrawApi::clearColor(r, g, b, a);
	}

	void Renderer3D::setViewPortSize(float width, float height) {
		DrawApi::setViewPortSize(width, height);
	}

	unsigned int Renderer3D::getDrawCallsCount() {
		return this->drawCallsCount;
	}

	void Renderer3D::drawPointLight(PointLight light, glm::mat4 transform) {
		light.position = transform * glm::vec4(1.0f);
		this->pointLights.push_back(light);
	}

	void Renderer3D::drawDirectionalLight(DirectionalLight light) {
		this->directionalLights.push_back(light);
	}

	void Renderer3D::render() {
		vertexArray.bind();

		for (DrawCallBuffer* drawCallBuffer : this->drawCallBuffers) {
			// execute the draw call with the data
			this->vertexBuffer.pushData(drawCallBuffer->getVertices(), drawCallBuffer->getSizeOfVertives());
			this->indexBuffer.pushData(drawCallBuffer->getIndices(), drawCallBuffer->getSizeOfIndices());
			DrawApi::drawWithIdexes(drawCallBuffer->getIndexCount());
			this->drawCallsCount++;
		} 

	}

	void Renderer3D::clearDrawCallBuffers() {
		for (DrawCallBuffer* drawCallBuffer : this->drawCallBuffers) {
			delete drawCallBuffer;
		}

		this->drawCallBuffers.clear();
	}

	void Renderer3D::updatePointLightsUniforms() {
		shaderProgram.setIntUniform("numberOfPointLights", pointLights.size());

		for (int i = 0; i < pointLights.size(); i++) {

			auto& pointLight = pointLights[i];

			shaderProgram.setVec3Uniform(format("pointLights[%d].position", i), pointLight.position);
			shaderProgram.setVec3Uniform(format("pointLights[%d].ambient", i), pointLight.ambient);
			shaderProgram.setVec3Uniform(format("pointLights[%d].diffuse", i), pointLight.diffuse);
			shaderProgram.setVec3Uniform(format("pointLights[%d].specular", i), pointLight.specular);

			shaderProgram.setFloatUniform(format("pointLights[%d].constant", i), pointLight.constant);
			shaderProgram.setFloatUniform(format("pointLights[%d].linear", i), pointLight.linear);
			shaderProgram.setFloatUniform(format("pointLights[%d].quadratic", i), pointLight.quadratic);

			shaderProgram.setFloatUniform(format("pointLights[%d].farPlane", i), pointLight.farPlane);
			shaderProgram.setIntUniform(format("pointLights[%d].cubeMapSlotIndex", i), pointLight.depthMapCubeMap->getSlot() - Texture::maxTextures);
		}

		
	}

	void Renderer3D::updateDirectionalLightsUniforms() {
		shaderProgram.setIntUniform("numberOfDirectionalLights", directionalLights.size());

		for (int i = 0; i < directionalLights.size(); i++) {

			auto& directionalLight = directionalLights[i];

			shaderProgram.setVec3Uniform(format("directionalLights[%d].position", i), directionalLight.position);
			shaderProgram.setVec3Uniform(format("directionalLights[%d].ambient", i), directionalLight.ambient);
			shaderProgram.setVec3Uniform(format("directionalLights[%d].diffuse", i), directionalLight.diffuse);
			shaderProgram.setVec3Uniform(format("directionalLights[%d].specular", i), directionalLight.specular);
			shaderProgram.setMat4Uniform(format("directionalLights[%d].viewProjection", i), directionalLight.getViewProjectionMatrix());
			shaderProgram.setIntUniform(format("directionalLights[%d].textureSlotIndex", i), directionalLight.depthMapTexture->getSlot());
		}

	}

	void Renderer3D::updatePointLightsDepthBuffers() {
		cubeMapDepthMapShaderProgram.bind();

		for (int i = 0; i < pointLights.size(); i++) {

			auto& pointLight = pointLights[i];

			bindCubeMap(pointLight.depthMapCubeMap.get());

			cubeMapDepthMapShaderProgram.setVec3Uniform("lightPos", pointLight.position);
			cubeMapDepthMapShaderProgram.setFloatUniform("far_plane", pointLight.farPlane);

			for (int j = 0; j < pointLight.getViewProjectionMatrices().size(); j++) {
				auto shadowMatrix = pointLight.getViewProjectionMatrices()[j];
				cubeMapDepthMapShaderProgram.setMat4Uniform(format("shadowMatrices[%d]", j), shadowMatrix);
			}

			DrawApi::setViewPortSize(pointLight.depthMapCubeMap->getWidth(), pointLight.depthMapCubeMap->getHeight());

			pointLight.depthMapFrameBuffer->bind();
			DrawApi::clearDepthBuffer();

			DrawApi::cullFrontFace();

			this->render();

			pointLight.depthMapFrameBuffer->unbind();

			DrawApi::cullBackFace();
		}

	}

}