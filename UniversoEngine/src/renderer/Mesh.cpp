#include "Mesh.h"

#include <cstring>

// uncomment this for output
//#define OBJL_CONSOLE_OUTPUT
#include "OBJ_Loader/OBJ_Loader.h"

#include "../debug/Assert.h"

namespace engine {

	Mesh::Mesh(const std::string& filepath) {
		ASSERT_FILE_EXISTS(filepath);

		ASSERT_FILE_EXTENSION(filepath, { ".obj" });

		objl::Loader loader;
		bool objFileLoaded = loader.LoadFile(filepath);

		ASSERT(objFileLoaded, "Mesh file is invalid or is already open");

		for (const objl::Vertex otherVertex : loader.LoadedVertices) {
			Vertex myVertex;
			std::memcpy(&myVertex.position, &otherVertex.Position, sizeof(myVertex.position));
			std::memcpy(&myVertex.textureCoords, &otherVertex.TextureCoordinate, sizeof(myVertex.textureCoords));

			this->vertices.push_back(myVertex);
		}
	}

	std::vector<glm::vec3> Mesh::getVeticesPositions() {
		std::vector<glm::vec3> verticesPositions;

		for (auto vertex : this->vertices) {
		 	verticesPositions.push_back(vertex.position);
		}

		return verticesPositions;
	}

}
