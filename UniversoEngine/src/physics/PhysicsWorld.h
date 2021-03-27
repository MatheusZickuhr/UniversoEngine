#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "RigidBody.h"

namespace engine {

    class PhysicsWorld {

    private:
        const glm::vec3 gravityDirection = {0.0f, -1.0f, 0.0f};
        const float gravityMagnitude = 0.3;
        const glm::vec3 gravityForce = gravityDirection * gravityMagnitude;
        std::vector<RigidBody*> rigidBodies;        

    public:
        void update(float deltaTime);

        void appendRigidBody(RigidBody* rigidBody);

        void removeRigidBody(RigidBody* rigidBody);

        void clear();

    private:
        int getCollisionCount(RigidBody* rigidBody);
    };
}