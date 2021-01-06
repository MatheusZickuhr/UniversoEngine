#include "engine.h"

class VertexBuffer {

private:
	unsigned int id;

public:
	VertexBuffer(unsigned int size, void* data) {
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);

		// add the data to the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};
