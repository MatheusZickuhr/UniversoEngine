#include "Renderer3D.h"

namespace engine {

	Renderer3D::Renderer3D() {
		// light
		this->lightVerticesBegin = new LightVertex[36];
		this->lightVertices = this->lightVerticesBegin;
		this->lightIndices = new unsigned int[36];

		for (int i = 0; i < 36; i++)
			lightIndices[i] = i;

		lightVertexArray.addVertexBuffer(lightVertexBuffer);
		lightVertexArray.addIndexBuffer(lightIndexBuffer);

		lightVertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(LightVertex, position));

		lightShaderProgram.attachShader(lightVertexShader.getId());
		lightShaderProgram.attachShader(lightFragShader.getId());
		lightShaderProgram.bind();

		// rest

		this->verticesBegin = new Vertex[maxVertices];
		this->vertices = this->verticesBegin;
		this->indices = new unsigned int[maxIndices];

		vertexArray.addVertexBuffer(vertexBuffer);
		vertexArray.addIndexBuffer(indexBuffer);

		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, position));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, color));
		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, normal));
		vertexBuffer.addAttributePointer(AttriuteType::Vec2, offsetof(Vertex, textureCoords));
		vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, textureSlot));

		shaderProgram.attachShader(vertexShader.getId());
		shaderProgram.attachShader(fragShader.getId());
		shaderProgram.bind();

		// add the textures slots to the shader
		int textureSlots[Texture::maxTextureSlot];
		for (int i = 0; i < Texture::maxTextureSlot; i++) textureSlots[i] = i;
		this->shaderProgram.setUniform1iv("textureSlots", Texture::maxTextureSlot, textureSlots);

		shaderProgram.setUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
		

		shaderProgram.setUniform3f("material.ambient", 1.0f, 0.5f, 0.31f);
		shaderProgram.setUniform3f("material.diffuse", 1.0f, 0.5f, 0.31f);
		shaderProgram.setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
		shaderProgram.setUniform1f("material.shininess", 32.0f);

		
		shaderProgram.setUniform3f("light.position", lightPosition.x, lightPosition.y, lightPosition.z);
		shaderProgram.setUniform3f("light.ambient", 0.2f, 0.2f, 0.2f);
		shaderProgram.setUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f);
		shaderProgram.setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

		shaderProgram.setUniform1f("light.constant", 1.0f);
		shaderProgram.setUniform1f("light.linear", 0.09f);
		shaderProgram.setUniform1f("light.quadratic", 0.032f);
		
	}

	Renderer3D::~Renderer3D() {
		delete[] this->verticesBegin;
		delete[] this->indices;
		delete[] this->lightVerticesBegin;
		delete[] this->lightIndices;
	}

	void Renderer3D::drawLightSource(glm::mat4 mvp) {

		
		this->lightShaderProgram.setUniformMat4f("Mvp", mvp);

		lightVertices = lightVerticesBegin;

		for (auto& vertex : lightMesh.getVertices()) {
			lightVertices->position = lightPosition +  vertex.position;
			lightVertices++;
		}
			
		lightShaderProgram.bind();
		lightVertexArray.bind();

		lightVertexBuffer.pushData(this->lightVerticesBegin, sizeof(LightVertex) * 36);
		lightIndexBuffer.pushData(this->lightIndices, sizeof(unsigned int) * 36);
		
		
		this->drawApi.drawWithIdexes(36);

	}

	void Renderer3D::startDrawing(glm::mat4 mvp, glm::vec3 cameraPosition) {
		shaderProgram.bind();
		this->shaderProgram.setUniformMat4f("Mvp", mvp);
		this->shaderProgram.setUniform3f("viewPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		this->drawCallsCount = 0;
	}

	void Renderer3D::endDrawing() {
		this->performDrawCall();
	}

	void Renderer3D::drawMesh(Mesh* mesh, Material* material, glm::mat4 transform) {

		Texture* texture = material->getTexture();
		glm::vec3 color = material->getColor();

		if (this->vertexCount + mesh->getVertexCount() > maxVertices)
			this->performDrawCall();
		
		for (const Vertex& vertex : mesh->getVertices()) {
			this->vertices->position = transform * glm::vec4(vertex.position, 1.0f);
			this->vertices->color = color;
			this->vertices->normal = glm::mat3(glm::transpose(glm::inverse(transform))) * vertex.normal;
			this->vertices->textureCoords = vertex.textureCoords;
			this->vertices->textureSlot = texture != nullptr ? texture->getSlot() : -1.0f;
			this->vertices++;
		}

		for (int i = this->indexCount; i < mesh->getVertexCount() + this->indexCount; i++)
			this->indices[i] = i;
		
		this->vertexCount += mesh->getVertexCount();
		this->indexCount += mesh->getVertexCount();
	}

	void Renderer3D::clear(float r, float g, float b, float a) {
		this->drawApi.clear(r, g, b, a);
	}

	void Renderer3D::setViewPortSize(float width, float height) {
		this->drawApi.setViewPortSize(width, height);
	}

	unsigned int Renderer3D::getDrawCallsCount() {
		return this->drawCallsCount;
	}

	void Renderer3D::performDrawCall() {
		vertexArray.bind();

		this->vertexBuffer.pushData(this->verticesBegin, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer.pushData(this->indices, sizeof(unsigned int) * this->indexCount);
		this->drawApi.drawWithIdexes(this->indexCount);

		this->vertices = this->verticesBegin;
		this->vertexCount = 0;
		this->indexCount = 0;

		this->drawCallsCount++;
	}
}