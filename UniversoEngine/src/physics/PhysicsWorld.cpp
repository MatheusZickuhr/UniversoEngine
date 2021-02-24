#include "PhysicsWorld.h"

namespace engine {

    void PhysicsWorld::update(float deltaTime) {

        for (auto rigidBody : this->rigidBodies) {
            if (rigidBody->isStatic) continue;

            rigidBody->applyForce(gravityForce);
            rigidBody->updatePosition(deltaTime);
        }
    }

    void PhysicsWorld::appendRigidBody(std::shared_ptr<RigidBody> rigidBody) {
        this->rigidBodies.push_back(rigidBody);
    }

    void PhysicsWorld::clear() {
        this->rigidBodies.clear();
    }
}