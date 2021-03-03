#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CollisionMesh.h"
#include "../math/Transform.h"
#include "TriangleIntersection.h"

namespace engine {

    class RigidBody {

    private:
        const float maxVelocity = 10.0f;  
        const float minVelocity = -maxVelocity; 

    public:
        float mass;
        bool isStatic;
        glm::vec3 acceleration;
        glm::vec3 velocity;
        glm::vec3 prevVelocity;
        glm::vec3 prevPosition;
        std::shared_ptr<CollisionMesh> collisionMesh;
        std::shared_ptr<Transform> transform;
        std::vector<glm::vec3> toBeAppliedForces;

        RigidBody(std::shared_ptr<Transform> trasnform, std::shared_ptr<CollisionMesh> collisionMesh);

        void moveToNextState();

        void moveToPrevState();

        void addForce(glm::vec3 force);

        void applyForce(glm::vec3 force);

        void update(); 

        bool collidesWith(std::shared_ptr<RigidBody> other);
    };
}