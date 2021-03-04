#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "RigidBody.h"

namespace engine {

    class PhysicsWorld {

    private:
        const glm::vec3 gravityDirection = {0.0f, -1.0f, 0.0f};
        const float gravityMagnitude = 10.0;
        const glm::vec3 gravityForce = gravityDirection * gravityMagnitude;
        std::vector<std::shared_ptr<RigidBody>> rigidBodies;        

    public:
        void update(float deltaTime);

        void appendRigidBody(std::shared_ptr<RigidBody> rigidBody);

        void clear();

    private:
        int getCollisionCount(std::shared_ptr<RigidBody> rigidBody);
    };
}