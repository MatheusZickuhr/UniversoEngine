

class Drawer {

public:

	Drawer() {
	
		
	}

	void drawWithIdexes(VertexArray* vertexArray, unsigned int IndexCount) {
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
	}
};