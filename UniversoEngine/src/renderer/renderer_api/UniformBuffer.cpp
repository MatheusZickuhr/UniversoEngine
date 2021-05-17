#include "UniformBuffer.h"
#include <glad/glad.h>

namespace engine {

	UniformBuffer::UniformBuffer(unsigned int bufferSize) {
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_UNIFORM_BUFFER, this->id);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &this->id);
	}

	void UniformBuffer::bind(unsigned int slot) {
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, this->id);
	}

	void UniformBuffer::pushData(void* data, unsigned int sizeOfData) {
		glBindBuffer(GL_UNIFORM_BUFFER, this->id);
		// offset, size, data
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeOfData, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}