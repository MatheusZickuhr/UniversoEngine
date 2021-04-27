#include "Renderer3D.h"
#include "../utils/Format.h"
#include <glad/glad.h>

namespace engine {

	Renderer3D::Renderer3D() {
		this->verticesBegin = new Vertex[maxVertices];
		this->vertices = this->verticesBegin;
		this->indices = new unsigned int[maxIndices];

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

	Renderer3D::~Renderer3D() {
		delete[] this->verticesBegin;
		delete[] this->indices;
	}

	void Renderer3D::startDepthDrawing(glm::mat4 mvp) {

		depthshaderProgram.bind();		
		
		DrawApi::setViewPortSize(depthMapTexture.getWidth(), depthMapTexture.getHeight());

		depthMapFrameBuffer.bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);
	}

	void Renderer3D::drawDepthMesh(Mesh* mesh, glm::mat4 transform) {
		
		if (this->vertexCount + mesh->getVertexCount() > maxVertices)
			this->performDrawCall();

		for (const Vertex& vertex : mesh->getVertices()) {
			this->vertices->position = transform * glm::vec4(vertex.position, 1.0f);
			this->vertices++;
		}

		for (int i = this->indexCount; i < mesh->getVertexCount() + this->indexCount; i++)
			this->indices[i] = i;

		this->vertexCount += mesh->getVertexCount();
		this->indexCount += mesh->getVertexCount();
	}

	void Renderer3D::endDepthDrawing() {
		this->performDrawCall();
		depthMapFrameBuffer.unbind();

		glCullFace(GL_BACK);
	}

	void Renderer3D::startDrawing(glm::mat4 mvp, glm::vec3 cameraPosition, const float width, const float height) {
		DrawApi::setViewPortSize(width, height);

		shaderProgram.bind();
		this->shaderProgram.setMat4Uniform("Mvp", mvp);
		this->shaderProgram.setVec3Uniform("viewPosition", cameraPosition);
		this->drawCallsCount = 0;


	}

	void Renderer3D::endDrawing() {
		this->performDrawCall();
	}

	void Renderer3D::drawMesh(Mesh* mesh, Material* material, glm::mat4 transform) {

		Texture* texture = material->getTexture();
		
		if (this->vertexCount + mesh->getVertexCount() > maxVertices)
			this->performDrawCall();
		
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

	void Renderer3D::performDrawCall() {
		vertexArray.bind();

		this->vertexBuffer.pushData(this->verticesBegin, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer.pushData(this->indices, sizeof(unsigned int) * this->indexCount);
		DrawApi::drawWithIdexes(this->indexCount);

		this->vertices = this->verticesBegin;
		this->vertexCount = 0;
		this->indexCount = 0;

		this->drawCallsCount++;
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