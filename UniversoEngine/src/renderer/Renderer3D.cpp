#include "Renderer3D.h"
#include "../utils/Format.h"
#include <glad/glad.h>
#include <utility>

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
		int textureSlots[Texture::maxTextureSlot];
		for (int i = 0; i < Texture::maxTextureSlot; i++) textureSlots[i] = i;
		this->shaderProgram.setIntArrayUniform("textureSlots", Texture::maxTextureSlot, textureSlots);

		shaderProgram.setIntUniform("numberOfPointLights", 0);

		shaderProgram.setIntUniform("numberOfDirectionalLights", 0);


		// shadow stuff
		

		shaderProgram.setIntUniform("shadowMapTextureSlot", (int) depthMapTexture.getSlot());

		depthshaderProgram.attachShader(depthVertexShader.getId());
		depthshaderProgram.attachShader(depthFragShader.getId());

		this->depthMapFrameBuffer.addTextureAsDepthBuffer(this->depthMapTexture);

	}

	void Renderer3D::updateDepthBuffers() {
		depthshaderProgram.bind();

		DrawApi::setViewPortSize(depthMapTexture.getWidth(), depthMapTexture.getHeight());

		depthMapFrameBuffer.bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);

		this->render();

		depthMapFrameBuffer.unbind();

		glCullFace(GL_BACK);

	}

	void Renderer3D::startDrawing(glm::mat4 mvp, glm::vec3 cameraPosition, const float width, const float height) {
		DrawApi::setViewPortSize(width, height);

		shaderProgram.bind();
		this->shaderProgram.setMat4Uniform("Mvp", mvp);
		this->shaderProgram.setVec3Uniform("viewPosition", cameraPosition);
		this->drawCallsCount = 0;

		this->currentDrawCallBuffer = new DrawCallBuffer{ maxVertices, maxIndices };

		clearDrawCallBuffers();

	}

	void Renderer3D::endDrawing() {
		drawCallBuffers.push_back(currentDrawCallBuffer);
		this->render();
	}

	void Renderer3D::drawMesh(Mesh* mesh, Material* material, glm::mat4 transform) {
		
		if (!currentDrawCallBuffer->doesFit(mesh)) {
			
			drawCallBuffers.push_back(currentDrawCallBuffer);
			this->currentDrawCallBuffer = new DrawCallBuffer{ maxVertices, maxIndices };
		}
		
		currentDrawCallBuffer->addMesh(mesh, material, transform);
	}

	void Renderer3D::startLightsDrawing() {
		pointLights.clear();
		directionalLights.clear();
	}

	void Renderer3D::endLightsDrawing() {
		this->updatePointLightsUniforms();
		this->updateDirectionalLightsUniforms();
	}

	void Renderer3D::clear(float r, float g, float b, float a) {
		DrawApi::clear(r, g, b, a);
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

		for (auto& drawCallBuffer : this->drawCallBuffers) {
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
		}

		
	}

	void Renderer3D::updateDirectionalLightsUniforms() {
		shaderProgram.setIntUniform("numberOfDirectionalLights", directionalLights.size());

		for (int i = 0; i < directionalLights.size(); i++) {

			auto& directionalLight = directionalLights[i];

			depthshaderProgram.setMat4Uniform("lightSpaceMatrix", directionalLight.getViewProjectionMatrix());

			shaderProgram.setVec3Uniform(format("directionalLights[%d].position", i), directionalLight.position);
			shaderProgram.setVec3Uniform(format("directionalLights[%d].ambient", i), directionalLight.ambient);
			shaderProgram.setVec3Uniform(format("directionalLights[%d].diffuse", i), directionalLight.diffuse);
			shaderProgram.setVec3Uniform(format("directionalLights[%d].specular", i), directionalLight.specular);
			shaderProgram.setMat4Uniform(format("directionalLights[%d].viewProjection", i), directionalLight.getViewProjectionMatrix());
		}

	}

}