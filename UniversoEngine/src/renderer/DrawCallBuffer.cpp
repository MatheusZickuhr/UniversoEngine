#include "DrawCallBuffer.h"
#include "../debug/Assert.h"

namespace engine {
	DrawCallBuffer::DrawCallBuffer(DrawCallBufferAllocator& drawCallBufferAllocator, unsigned int maxVertices, unsigned int maxIndices)
		: maxVertices(maxVertices), maxIndices(maxIndices) {

		this->vertices = drawCallBufferAllocator.allocateVertices(maxVertices);
		this->verticesBegin = vertices;
		this->indices = drawCallBufferAllocator.allocateIndices(maxIndices);
	}

	void DrawCallBuffer::addMesh(Mesh* mesh, Material* material, glm::mat4 transform) {
		ASSERT(this->doesFit(mesh), "The mesh does not fit in the drawCall");

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



