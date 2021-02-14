#pragma once

#include <glad/glad.h>

namespace engine {

	class VertexBuffer {

	private:
		unsigned int id;

	public:
		VertexBuffer(unsigned int size, void* data);

		VertexBuffer(unsigned int size);

		~VertexBuffer();
		
		void bind();
		
		void unbind();

		void addLayout(unsigned int location, unsigned int count, unsigned int stride, unsigned int offset);

		void pushData(void* data, unsigned int size);
	};


}