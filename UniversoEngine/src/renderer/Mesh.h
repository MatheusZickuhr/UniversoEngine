#pragma once

#include <vector>
#include <string>
#include "Vertex.h"

namespace engine {

	class Mesh {

	public:


		Mesh(const std::string& filepath, bool useIndices = true);

		uint32_t getVertexCount() { return (uint32_t) this->vertices.size(); }

		const std::vector<Vertex>& getVertices() { return this->vertices; }

		uint32_t getIndexCount() { return (uint32_t) this->indices.size(); }

		const std::vector<int32_t>& getIndices() { return this->indices; }

	private:
		std::vector<Vertex> vertices;
		std::vector<int32_t> indices;


		int32_t findVertexIndex(Vertex vertex);
	};
}