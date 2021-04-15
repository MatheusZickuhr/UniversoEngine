#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"


namespace engine {
	
	class VertexArray {

	public:
		VertexArray();

		~VertexArray();

		void bind();

		void unbind();

		void addIndexBuffer(IndexBuffer& indexBuffer);

		void addVertexBuffer(VertexBuffer& vertexBuffer);

	private:

		unsigned int id;

	};
}