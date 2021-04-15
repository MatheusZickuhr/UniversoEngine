#include "VertexArray.h"
#include <glad/glad.h>

namespace engine {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &this->id);
		glBindVertexArray(this->id);
	}
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &this->id);
	}

	void VertexArray::bind() {
		glBindVertexArray(this->id);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::addIndexBuffer(IndexBuffer& indexBuffer) {
		this->bind();
		indexBuffer.bind();
	}

	void VertexArray::addVertexBuffer(VertexBuffer& vertexBuffer) {
		this->bind();
		vertexBuffer.bind();
	}

}