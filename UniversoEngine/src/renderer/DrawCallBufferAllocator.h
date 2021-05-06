#pragma once
#include "Vertex.h"

namespace engine {

	// this class allocates memory for drawcall buffers
	// todo: allocate more memory if need, instead of using a max value with an assert
	class DrawCallBufferAllocator {

	public:

		DrawCallBufferAllocator(unsigned int maxVertices, unsigned int maxIndices);

		DrawCallBufferAllocator(const DrawCallBufferAllocator& other) = delete;

		~DrawCallBufferAllocator();

		Vertex* allocateVertices(int vertexCount);

		unsigned int* allocateIndices(int indexCount);

		void clear();

	private:

		Vertex* vertexBufferBegin;
		Vertex* vertexBuffer;

		unsigned int* indexBuffer;
		unsigned int* indexBufferBegin;

		unsigned int allocatedVertices;
		unsigned int allocatedIndices;

		const unsigned int maxVertices;
		const unsigned int maxIndices;
	};

}
