
class VertexArray {

private:
	unsigned int id;

public:
	VertexArray() {
		glGenVertexArrays(1, &this->id);
		glBindVertexArray(this->id);
	}
	~VertexArray() {
		glDeleteVertexArrays(1, &this->id);
	}

	void bind() {
		glBindVertexArray(this->id);
	}

	void unbind() {
		glBindVertexArray(0);
	}
};