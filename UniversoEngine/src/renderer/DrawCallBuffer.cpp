#include "DrawCallBuffer.h"

namespace engine {
	DrawCallBuffer::DrawCallBuffer(unsigned int maxVertices, unsigned int maxIndices)
		: maxVertices(maxVertices), maxIndices(maxIndices) {

		this->verticesBegin = new Vertex[maxVertices];
		this->vertices = this->verticesBegin;
		this->indices = new unsigned int[maxIndices];
	}

	DrawCallBuffer::~DrawCallBuffer() {
		delete[] this->verticesBegin;
		delete[] this->indices;
	}

	void DrawCallBuffer::addMesh(Mesh* mesh, Material* material, glm::mat4 transform) {

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

	void DrawCallBuffer::addMesh(Mesh* mesh, glm::mat4 transform) {

		for (const Vertex& vertex : mesh->getVertices()) {
			this->vertices->position = transform * glm::vec4(vertex.position, 1.0f);
			this->vertices->normal = glm::mat3(glm::transpose(glm::inverse(transform))) * vertex.normal;
			this->vertices->textureCoords = vertex.textureCoords;
			this->vertices++;
		}

		for (int i = this->indexCount; i < mesh->getVertexCount() + this->indexCount; i++)
			this->indices[i] = i;

		this->vertexCount += mesh->getVertexCount();
		this->indexCount += mesh->getVertexCount();
	}

	bool DrawCallBuffer::doesFit(Mesh* mesh) {
		return this->vertexCount + mesh->getVertexCount() <= this->maxVertices;
	}

	void DrawCallBuffer::clear() {
		this->indexCount = 0;
		this->vertexCount = 0;
		this->vertices = this->verticesBegin;
	}

	Vertex* DrawCallBuffer::getVertices() {
		return verticesBegin;
	}

	unsigned int* DrawCallBuffer::getIndices() {
		return indices;
	}

	unsigned int DrawCallBuffer::getSizeOfVertives() {
		return sizeof(Vertex) * this->vertexCount;
	}

	unsigned int DrawCallBuffer::getSizeOfIndices() {
		return sizeof(unsigned int) * this->indexCount;
	}

	unsigned int DrawCallBuffer::getIndexCount() {
		return this->indexCount;
	}

}



