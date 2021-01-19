

class Drawer {

public:

	Drawer() {


	}

	void drawWithIdexes(std::shared_ptr<VertexArray> vertexArray, unsigned int IndexCount) {
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
	}


	void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

};