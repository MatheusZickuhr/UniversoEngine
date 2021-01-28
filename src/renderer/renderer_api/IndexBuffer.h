
namespace engine {

	class IndexBuffer {
	private:
		unsigned int id;
		unsigned int count;

	public:

		IndexBuffer(unsigned int* data, unsigned int count) {
			this->count = count;
			glGenBuffers(1, &this->id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
		}

		IndexBuffer(unsigned int count) {
			this->count = count;
			glGenBuffers(1, &this->id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
		}

		~IndexBuffer() {
			glDeleteBuffers(1, &this->id);
		}

		void pushData(unsigned int* data, unsigned int size) {
			this->bind();
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
		}

		void bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		}

		void unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		unsigned int getCount() {
			return this->count;
		}
	};
}