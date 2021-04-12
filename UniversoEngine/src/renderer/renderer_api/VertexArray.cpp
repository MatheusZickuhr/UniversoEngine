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

}