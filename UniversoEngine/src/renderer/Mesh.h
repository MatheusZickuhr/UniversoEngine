#pragma once

#include <vector>
#include "Vertex.h"

namespace engine {

	class Mesh {

	public:

		std::vector<Vertex> vertices;

		Mesh(const char* filepath);

		std::vector<glm::vec3> getVeticesPositions();
	};
}