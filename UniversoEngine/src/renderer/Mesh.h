#pragma once

#include <vector>
#include "Vertex.h"

namespace engine {

	class Mesh {

	public:

		std::vector<Vertex> vertices;

		Mesh(const char* filepath);
	};
}