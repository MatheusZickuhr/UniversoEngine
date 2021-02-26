#include "PhysicsWorld.h"

namespace engine {

    void PhysicsWorld::update(float deltaTime) {

        for (auto rigidBody : this->rigidBodies) {
            if (rigidBody->isStatic || this->isColliding(rigidBody)) continue;

            rigidBody->applyForce(gravityForce);
            rigidBody->updatePosition(deltaTime);
        }
    }

    bool PhysicsWorld::isColliding(std::shared_ptr<RigidBody> targetRigidBody) {

        for (auto otherRigidBody: this->rigidBodies) {

            if (targetRigidBody == otherRigidBody) {
                continue;
            }

            for (int i = 0; i < targetRigidBody->collisionMesh->vertices.size(); i+=3) {

                Triangle targetTriangle = {
                    targetRigidBody->collisionMesh->vertices[i+0] + targetRigidBody->position,
                    targetRigidBody->collisionMesh->vertices[i+1] + targetRigidBody->position,
                    targetRigidBody->collisionMesh->vertices[i+2] + targetRigidBody->position
                };

                for (int j = 0; j < otherRigidBody->collisionMesh->vertices.size(); j+=3) {
                    Triangle otherTriangle = {
                        otherRigidBody->collisionMesh->vertices[j+0] + otherRigidBody->position,
                        otherRigidBody->collisionMesh->vertices[j+1] + otherRigidBody->position,
                        otherRigidBody->collisionMesh->vertices[j+2] + otherRigidBody->position
                    };

                    auto doesCollid = trianglesIntersects(&targetTriangle, &otherTriangle);

                    if (doesCollid)
                        return true;
                }
            }
        }

        return false;
    }

    void PhysicsWorld::appendRigidBody(std::shared_ptr<RigidBody> rigidBody) {
        this->rigidBodies.push_back(rigidBody);
    }

    void PhysicsWorld::clear() {
        this->rigidBodies.clear();
    }
}