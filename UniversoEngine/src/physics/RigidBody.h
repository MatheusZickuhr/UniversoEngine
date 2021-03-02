#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "CollisionMesh.h"
#include "../math/Transform.h"

namespace engine {

    class RigidBody {

    private:
        const float maxVelocity = 10.0f;  
        const float minVelocity = -maxVelocity; 

    public:
        float mass;
        glm::vec3 accelaration;
        glm::vec3 velocity;
        bool isStatic;
        std::shared_ptr<CollisionMesh> collisionMesh;
        std::shared_ptr<Transform> transform;
        
        RigidBody(std::shared_ptr<Transform> trasnform, std::shared_ptr<CollisionMesh> collisionMesh);

        void applyForce(glm::vec3 force);

        void update(float deltaTime); 

    };
}