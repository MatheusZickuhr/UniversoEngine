#include "Renderer3D.h"

namespace engine {

	Renderer3D::Renderer3D() {
		this->vertexCount = 0;
		this->indexCount = 0;

		this->verticesPtrStart = new Vertex[maxVertices];
		this->vertices = this->verticesPtrStart;
		this->indices = new unsigned int[maxIndices];

		this->vertexArray = std::make_shared<VertexArray>();
		this->createVetexBuffer();
		this->createIndexBuffer();

		this->loadShaders();

		this->setupShaderUniforms();


		this->drawer = std::make_unique<Drawer>();
	}

	Renderer3D::~Renderer3D() {
		delete[] this->verticesPtrStart;
		delete[] this->indices;
	}

	void Renderer3D::startDrawing(glm::mat4 mvp) {
		this->shaderProgram->setUniformMat4f("Mvp", mvp);

		this->vertices = this->verticesPtrStart;
		this->vertexCount = 0;
		this->indexCount = 0;
	}

	void Renderer3D::endDrawing() {
		this->vertexBuffer->pushData(this->verticesPtrStart, sizeof(Vertex) * this->vertexCount);
		this->indexBuffer->pushData(this->indices, sizeof(unsigned int) * this->indexCount);
		this->drawer->drawWithIdexes(this->vertexArray, this->indexCount);
	}

	void Renderer3D::drawMesh(
		Mesh* mesh,
		Texture* texture,
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec3 rotationAxis,
		float rotationAngle
	) {

		float textureSlot;

		if (texture != nullptr) {
			textureSlot = texture->getSlot();
			texture->bind();
		}
		else {
			textureSlot = -1.0f;
		}

		for (const Vertex vertex : mesh->vertices) {
			this->vertices->position = (vertex.position + position) * scale;
			this->vertices->textureCoords = vertex.textureCoords;
			this->vertices->textureSlot = textureSlot;
			this->vertices->rotationAngle = rotationAngle;
			this->vertices->rotationAxis = rotationAxis;
			this->vertices++;
		}

		for (int i = this->indexCount; i < mesh->vertices.size() + this->indexCount; i++) {
			this->indices[i] = i;
		}

		this->vertexCount += mesh->vertices.size();
		this->indexCount += mesh->vertices.size();
	}

	void Renderer3D::clear(float r, float g, float b, float a) {
		this->drawer->clear(r, g, b, a);
	}

	void Renderer3D::createVetexBuffer() {
		this->vertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * maxVertices);
		this->vertexBuffer->addLayout(0, 3, sizeof(Vertex), offsetof(Vertex, position));
		this->vertexBuffer->addLayout(1, 2, sizeof(Vertex), offsetof(Vertex, textureCoords));
		this->vertexBuffer->addLayout(2, 1, sizeof(Vertex), offsetof(Vertex, textureSlot));
		this->vertexBuffer->addLayout(3, 1, sizeof(Vertex), offsetof(Vertex, rotationAngle));
		this->vertexBuffer->addLayout(4, 3, sizeof(Vertex), offsetof(Vertex, rotationAxis));
	}

	void Renderer3D::createIndexBuffer() {
		this->indexBuffer = std::make_unique<IndexBuffer>(maxIndices);
	}

	void Renderer3D::loadShaders() {
		this->vertexShader = std::make_unique<Shader>(VertexShader, "res/shaders/3d/vert.glsl");
		this->fragShader = std::make_unique<Shader>(FragmentShader, "res/shaders/3d/frag.glsl");
		this->shaderProgram = std::make_unique<ShaderProgram>();
		shaderProgram->attachShader(vertexShader->getId());
		shaderProgram->attachShader(fragShader->getId());
		shaderProgram->bind();
	}

	void Renderer3D::setupShaderUniforms() {

		const unsigned int maxTextures = 32;
		int textureSlots[maxTextures];
		for (int i = 0; i < maxTextures; i++) textureSlots[i] = i;
		this->shaderProgram->setUniform1iv("textureSlots", maxTextures, textureSlots);
	}

}