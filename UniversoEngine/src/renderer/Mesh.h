#pragma once

#include <vector>
#include <string>
#include "Vertex.h"

namespace engine {

	class Mesh {

	public:


		Mesh(const std::string& filepath);

		int getVertexCount() { return this->vertices.size(); }

		const std::vector<Vertex>& getVertices() { return this->vertices; }

		int getIndexCount() { return this->indices.size(); }

		const std::vector<int>& getIndices() { return this->indices; }

	private:
		std::vector<Vertex> vertices;
		std::vector<int> indices;


		int findVertexIndex(Vertex vertex);
	};
}