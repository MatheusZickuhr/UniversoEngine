#pragma once

#include <vector>
#include <string>
#include "Vertex.h"

namespace engine {

	class Mesh {

	public:


		Mesh(const std::string& filepath);

		int getVertexCount();

		const std::vector<Vertex>& getVertices();

	private:
		std::vector<Vertex> vertices;

	};
}