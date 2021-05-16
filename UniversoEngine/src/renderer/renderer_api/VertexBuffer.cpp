#include "VertexBuffer.h"
#include <glad/glad.h>

namespace engine {

	VertexBuffer::VertexBuffer(unsigned int vertexSize, unsigned int count, void* data) {
		this->stride = vertexSize;

		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);

		const unsigned int bufferSize = vertexSize * count;
		glBufferData(GL_ARRAY_BUFFER, bufferSize, data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(unsigned int vertexSize, unsigned int count) {
		this->stride = vertexSize;

		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);

		const unsigned int bufferSize = vertexSize * count;
		glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
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

	void VertexBuffer::addAttributePointer(AttriuteType attriuteType, unsigned int offset) {
		this->bind();

		switch (attriuteType) {

		case AttriuteType::Float:
			glVertexAttribPointer(currentLocation, 1, GL_FLOAT, GL_FALSE, this->stride, (void*)offset);
			break;
		case AttriuteType::Vec2:
			glVertexAttribPointer(currentLocation, 2, GL_FLOAT, GL_FALSE, this->stride, (void*)offset);
			break;
		case AttriuteType::Vec3:
			glVertexAttribPointer(currentLocation, 3, GL_FLOAT, GL_FALSE, this->stride, (void*)offset);
			break;
		}
		
		glEnableVertexAttribArray(currentLocation);

		this->currentLocation++;
	}

	void VertexBuffer::pushData(void* data, unsigned int size) {
		this->bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}



}