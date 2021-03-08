#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
        std::vector<glm::vec3> collisionMesh;
        Transform transform;
        std::vector<glm::vec3> toBeAppliedForces;

        RigidBody(std::vector<glm::vec3> collisionMesh);

        void moveToNextState();

        void moveToPrevState();

        void addForce(glm::vec3 force);

        void applyForce(glm::vec3 force);

        void update(); 

        bool collidesWith(RigidBody* other);
    };
}