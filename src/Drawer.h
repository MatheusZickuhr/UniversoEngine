

class Drawer {

public:

	Drawer() {
	
		
	}

	void drawWithIdexes(std::shared_ptr<VertexArray> vertexArray, unsigned int IndexCount) {
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
	}
};