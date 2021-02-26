#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace engine {

    
    class CollisionMesh {
    
    public:

        std::vector<glm::vec3> vertices;
        CollisionMesh(std::vector<glm::vec3> vertices);
    };
}