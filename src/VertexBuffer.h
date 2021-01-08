

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
	~VertexBuffer() {
		glDeleteBuffers(1, &this->id);
	}
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*
	location: used inside the shader
	count: number of elements inside the atributes (example: vec3 (position) = 3)
	stride: length in bytes of each atribute (example: vec3 = 3x4 bytes or 12 bytes)
	offset: initial value in bytes where the first atribute is located
	*/
	void addLayout(unsigned int location, unsigned int count, unsigned int stride, unsigned int offset) {
		glVertexAttribPointer(location, count, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		glEnableVertexAttribArray(location);
	}
};
