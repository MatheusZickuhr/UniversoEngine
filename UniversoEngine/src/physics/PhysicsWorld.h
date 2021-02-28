#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "RigidBody.h"
#include "TriangleIntersection.h"

namespace engine {

    class PhysicsWorld {

    private:
        const glm::vec3 gravityForce = {0.0f, -5.0f, 0.0f};
        std::vector<std::shared_ptr<RigidBody>> rigidBodies;        

    public:
        void update(float deltaTime);

        void appendRigidBody(std::shared_ptr<RigidBody> rigidBody);

        void clear();

    private:
        bool isColliding(std::shared_ptr<RigidBody> rigidBody);
    };
}