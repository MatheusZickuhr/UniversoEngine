#include "PhysicsWorld.h"

namespace engine {

    void PhysicsWorld::update(float deltaTime) {

        for (RigidBody* rigidBody : this->rigidBodies) {
            rigidBody->applyForce(gravityForce);
            rigidBody->update(deltaTime);
        }
    }

    void PhysicsWorld::appendRigidBody(RigidBody *rigidBody) {
        this->rigidBodies.push_back(rigidBody);
    }

    void PhysicsWorld::clear() {
        this->rigidBodies.clear();
    }
}