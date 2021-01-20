#include "OBJ_Loader/OBJ_Loader.h"
#include <vector>


class Mesh {


public:

	std::vector<Vertex> vertices;

	Mesh(const char * filepath) {
		objl::Loader loader;
		loader.LoadFile(filepath);
		
		for (const objl::Vertex otherVertex: loader.LoadedVertices) {
			Vertex myVertex;
			memcpy(&myVertex.position, &otherVertex.Position, sizeof(myVertex.position) );
			this->vertices.push_back(myVertex);
		}
	}
};