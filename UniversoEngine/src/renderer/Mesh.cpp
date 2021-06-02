#include "Mesh.h"

#include <cstring>

// uncomment this for output
//#define OBJL_CONSOLE_OUTPUT
#include "OBJ_Loader/OBJ_Loader.h"

#include "../debug/Assert.h"

namespace engine {

	Mesh::Mesh(const std::string& filepath, bool useIndices) {
		ASSERT_FILE_EXISTS(filepath);
		ASSERT_FILE_EXTENSION(filepath, { ".obj" });

		objl::Loader loader;
		bool objFileLoaded = loader.LoadFile(filepath);

		ASSERT(objFileLoaded, "Mesh file is invalid or is already open");
		
		for (const objl::Vertex& otherVertex : loader.LoadedVertices) {
			Vertex myVertex;
			std::memcpy(&myVertex.position, &otherVertex.Position, sizeof(myVertex.position));
			std::memcpy(&myVertex.normal, &otherVertex.Normal, sizeof(myVertex.normal));
			std::memcpy(&myVertex.textureCoords, &otherVertex.TextureCoordinate, sizeof(myVertex.textureCoords));

			if (!useIndices) {
				this->vertices.push_back(myVertex);
				continue;
			}

			// add only unique vertices to the vertex list
			int vertexIndex = this->findVertexIndex(myVertex);

			if (vertexIndex == -1) {
				this->vertices.push_back(myVertex);
				this->indices.push_back(this->vertices.size() - 1);
			}
			else {
				this->indices.push_back(vertexIndex);
			}
		}
	}

	int Mesh::findVertexIndex(Vertex vertex) {
		for (int i = 0; i < vertices.size(); i++) {
			if (vertex.position == vertices[i].position
				&& vertex.textureCoords == vertices[i].textureCoords
				&& vertex.normal == vertices[i].normal) {
				return i;
			}
		}
		return -1;
	}

}
