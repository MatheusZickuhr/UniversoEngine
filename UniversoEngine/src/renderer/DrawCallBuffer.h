#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "DrawCallBufferAllocator.h"

namespace engine {

	//this class represents all the vertex/index data that will be send to gpu in a draw call
	class DrawCallBuffer {

	public:

		DrawCallBuffer(DrawCallBufferAllocator&, unsigned int, unsigned int);

		void addMesh(Mesh* mesh, Material* material, glm::mat4 transform);

		void addMesh(Mesh* mesh, glm::mat4 transform);

		bool doesFit(Mesh* mesh);

		Vertex* getVertices();

		unsigned int* getIndices();

		unsigned int getSizeOfVertives();

		unsigned int getSizeOfIndices();

		unsigned int getIndexCount();

	private:

		unsigned int maxVertices;
		unsigned int maxIndices;

		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;

		Vertex* vertices;
		Vertex* verticesBegin;
		unsigned int* indices;

	};
}
