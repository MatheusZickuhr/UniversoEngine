#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "CollisionMesh.h"

namespace engine {

    class RigidBody {
    private:
        float mass;
        glm::vec3 accelaration;
        glm::vec3 velocity;
        

    public:
        bool isStatic;
        std::shared_ptr<CollisionMesh> collisionMesh;

        glm::vec3 position;

        RigidBody(glm::vec3 position  = {0.0f, 0.0f, 0.0f});

        void applyForce(glm::vec3 force);

        void updatePosition(float deltaTime);

    };
}