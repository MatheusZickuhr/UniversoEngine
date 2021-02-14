#include "VertexBuffer.h"

namespace engine {

	VertexBuffer::VertexBuffer(unsigned int size, void* data) {
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);

		// add the data to the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(unsigned int size) {
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);

		// alocate memory in the gpu
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &this->id);
	}
	void VertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}
	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::addLayout(unsigned int location, unsigned int count, unsigned int stride, unsigned int offset) {
		glVertexAttribPointer(location, count, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		glEnableVertexAttribArray(location);
	}

	void VertexBuffer::pushData(void* data, unsigned int size) {
		this->bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}



}