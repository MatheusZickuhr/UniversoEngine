#pragma once

#include <glad/glad.h>

namespace engine {

	class IndexBuffer {
	private:
		unsigned int id;
		unsigned int count;

	public:

		IndexBuffer(unsigned int* data, unsigned int count);

		IndexBuffer(unsigned int count);

		~IndexBuffer();
		void pushData(unsigned int* data, unsigned int size);

		void bind();

		void unbind();

		unsigned int getCount();
	};
}