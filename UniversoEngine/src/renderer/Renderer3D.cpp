#include "Renderer3D.h"

namespace engine {


	Renderer3D::Renderer3D() {
		this->verticesBegin = new Vertex[maxVertices];
		this->vertices = this->verticesBegin;
		this->indices = new unsigned int[maxIndices];

		vertexArray.addVertexBuffer(vertexBuffer);
		vertexArray.addIndexBuffer(indexBuffer);

		vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(Vertex, position));
		vertexBuffer.addAttributePointer(AttriuteType::Vec2, offsetof(Vertex, textureCoords));
		vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(Vertex, textureSlot));

		shaderProgram.attachShader(vertexShader.getId());
		shaderProgram.attachShader(fragShader.getId());
		shaderProgram.bind();

		// add the textures slots to the shader
		int textureSlots[Texture::maxTextureSlot];
		for (int i = 0; i < Texture::maxTextureSlot; i++) textureSlots[i] = i;
		this->shaderProgram.setUniform1iv("textureSlots", Texture::maxTextureSlot, textureSlots);
	}

	Renderer3D::~Renderer3D() {
		delete[] this->verticesBegin;
		delete[] this->indices;
	}

	void Renderer3D::startDrawing(glm::mat4 mvp) {
		this->shaderProgram.setUniformMat4f("Mvp", mvp);
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
		this->vertexBuffer.pushData(this->verticesBegin, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer.pushData(this->indices, sizeof(unsigned int) * this->indexCount);
		this->drawApi.drawWithIdexes(this->vertexArray, this->indexCount);

		this->vertices = this->verticesBegin;
		this->vertexCount = 0;
		this->indexCount = 0;

		this->drawCallsCount++;
	}
}