#pragma once

namespace engine {

	class IndexBuffer {

	public:

		IndexBuffer(unsigned int* data, unsigned int count);

		IndexBuffer(unsigned int count);

		IndexBuffer(const IndexBuffer& other) = delete;

		~IndexBuffer();
		
		void pushData(unsigned int* data, unsigned int size);

		void bind();

		void unbind();

		unsigned int getCount();

	private:

		unsigned int id;
		unsigned int count;
	};
}