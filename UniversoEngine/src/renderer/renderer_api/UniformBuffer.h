#pragma once

namespace engine {

	class UniformBuffer {

	public:

		UniformBuffer(unsigned int bufferSize);

		UniformBuffer(const UniformBuffer& other) = delete;

		~UniformBuffer();

		void bind(unsigned int slot);
		
		void pushData(void* data, unsigned int sizeOfData);

	private:

		unsigned int id;
	};

}
