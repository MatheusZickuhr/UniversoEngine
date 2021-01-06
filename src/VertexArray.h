#include "engine.h"

class VertexArray {

private:
	unsigned int id;

public:
	VertexArray() {
		glGenVertexArrays(1, &this->id);
		glBindVertexArray(this->id);
	}

	void bind() {
		glBindVertexArray(this->id);
	}

	void unbind() {
		glBindVertexArray(0);
	}
};