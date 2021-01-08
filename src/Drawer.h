

class Drawer {

public:

	Drawer() {
	
		
	}

	void drawWithIdexes(VertexArray* vertexArray, IndexBuffer* indexBuffer) {
		vertexArray->bind();
		indexBuffer->bind();
		glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
	}
};