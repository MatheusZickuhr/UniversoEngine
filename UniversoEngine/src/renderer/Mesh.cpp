#include "Mesh.h"

#include <cstring>

// uncomment this for output
//#define OBJL_CONSOLE_OUTPUT
#pragma warning(push, 0)        
#include "OBJ_Loader/OBJ_Loader.h"
#pragma warning(pop)

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
			uint32_t vertexIndex = this->findVertexIndex(myVertex);

			// if is a new vertex
			if (vertexIndex == (uint32_t)vertices.size()) {
				this->vertices.push_back(myVertex);
			}

			this->indices.push_back(vertexIndex);	
		}
	}

	uint32_t Mesh::findVertexIndex(Vertex vertex) {
		for (uint32_t i = 0; i < (uint32_t) vertices.size(); i++) {
			if (vertex.position == vertices[i].position
				&& vertex.textureCoords == vertices[i].textureCoords
				&& vertex.normal == vertices[i].normal) {
				return i;
			}
		}
		return (uint32_t) vertices.size();
	}

}
