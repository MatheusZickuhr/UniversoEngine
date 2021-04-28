#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	class DrawCallBuffer {

	public:

		DrawCallBuffer(unsigned int maxVertices, unsigned int maxIndices);

		~DrawCallBuffer();

		void addMesh(Mesh* mesh, Material* material, glm::mat4 transform);

		void addMesh(Mesh* mesh, glm::mat4 transform);

		bool doesFit(Mesh* mesh);

		void clear();

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

		// always points to the begining of the vertex buffer
		Vertex* verticesBegin;
		Vertex* vertices;
		unsigned int* indices;

	};
}
