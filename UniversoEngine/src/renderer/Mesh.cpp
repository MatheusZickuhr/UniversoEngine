#include "Mesh.h"
#include "OBJ_Loader/OBJ_Loader.h"

namespace engine {

	Mesh::Mesh(const char* filepath) {
		objl::Loader loader;
		loader.LoadFile(filepath);

		for (const objl::Vertex otherVertex : loader.LoadedVertices) {
			Vertex myVertex;
			memcpy(&myVertex.position, &otherVertex.Position, sizeof(myVertex.position));
			memcpy(&myVertex.textureCoords, &otherVertex.TextureCoordinate, sizeof(myVertex.textureCoords));

			this->vertices.push_back(myVertex);
		}
	}

}