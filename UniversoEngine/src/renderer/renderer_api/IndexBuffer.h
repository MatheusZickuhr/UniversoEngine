#pragma once

#include <stdint.h>

namespace engine {

	class IndexBuffer {

	public:

		IndexBuffer(uint32_t* data, uint32_t count);

		IndexBuffer(uint32_t count);

		IndexBuffer(const IndexBuffer& other) = delete;

		~IndexBuffer();
		
		void pushData(uint32_t* data, uint32_t size);

		void bind();

		void unbind();

		uint32_t getCount();

	private:

		uint32_t id;
		uint32_t count;
	};
}