#pragma once

#include <vector>
#include <string>
#include "Vertex.h"

namespace engine {

	class Mesh {

	public:

		std::vector<Vertex> vertices;

		Mesh(const std::string& filepath);

		std::vector<glm::vec3> getVeticesPositions();
	};
}