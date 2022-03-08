#pragma once
#include <stdint.h>

namespace engine {

	class UniformBuffer {

	public:

		UniformBuffer(uint32_t bufferSize);

		UniformBuffer(const UniformBuffer& other) = delete;

		~UniformBuffer();

		void bind(uint32_t slot);
		
		void pushData(void* data, uint32_t sizeOfData);

	private:

		uint32_t id;
	};

}
