#include "DrawCallBufferAllocator.h"
#include "../debug/Assert.h"

namespace engine {

	DrawCallBufferAllocator::DrawCallBufferAllocator(unsigned int maxVertices, unsigned int maxIndices) :maxVertices(maxVertices), maxIndices(maxIndices) {
		this->allocatedVertices = 0;
		this->vertexBuffer = new Vertex[maxVertices];
		this->vertexBufferBegin = vertexBuffer;

		this->allocatedIndices = 0;
		this->indexBuffer = new unsigned int[maxIndices];
		this->indexBufferBegin = indexBuffer;
	}

	DrawCallBufferAllocator::~DrawCallBufferAllocator() {
		delete[] this->vertexBufferBegin;
		delete[] this->indexBufferBegin;
	}

	Vertex* DrawCallBufferAllocator::allocateVertices(int vertexCount) {
		ASSERT(this->allocatedVertices + vertexCount <= this->maxVertices, "Maximum vertex count exceded");

		this->allocatedVertices += vertexCount;

		Vertex* ptr = this->vertexBuffer;

		this->vertexBuffer += vertexCount;

		return ptr;
	}

	unsigned int* DrawCallBufferAllocator::allocateIndices(int indexCount) {
		ASSERT(this->allocatedIndices + indexCount <= this->maxIndices, "Maximum index count exceded");

		this->allocatedIndices += indexCount;

		unsigned int* ptr = this->indexBuffer;

		this->indexBuffer += indexCount;

		return ptr;
	}

	void DrawCallBufferAllocator::clear() {
		this->vertexBuffer = this->vertexBufferBegin;
		this->allocatedVertices = 0;

		this->indexBuffer = this->indexBufferBegin;
		this->allocatedIndices = 0;
	}

}
