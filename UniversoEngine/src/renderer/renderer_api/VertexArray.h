#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"


namespace engine {
	
	class VertexArray {

	public:
		VertexArray();

		VertexArray(const VertexArray& other) = delete;

		~VertexArray();

		void bind();

		void unbind();

		void addIndexBuffer(IndexBuffer& indexBuffer);

		void addVertexBuffer(VertexBuffer& vertexBuffer);

	private:

		uint32_t id;

	};
}