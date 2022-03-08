#include "IndexBuffer.h"
#include <glad/glad.h>

namespace engine {

	IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count) {
		this->count = count;
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(uint32_t count) {
		this->count = count;
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &this->id);
	}

	void IndexBuffer::pushData(uint32_t* data, uint32_t size) {
		this->bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
	}

	void IndexBuffer::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	}

	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t IndexBuffer::getCount() {
		return this->count;
	}

}