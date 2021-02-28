#include "PhysicsWorld.h"

namespace engine {

    void PhysicsWorld::update(float deltaTime) {

        for (auto rigidBody : this->rigidBodies) {
            rigidBody->update(deltaTime);
            
            if (rigidBody->isStatic || this->isColliding(rigidBody)) continue;

            rigidBody->applyForce(gravityForce);
        }
    }

    bool PhysicsWorld::isColliding(std::shared_ptr<RigidBody> targetRigidBody) {

        for (auto otherRigidBody: this->rigidBodies) {

            if (targetRigidBody == otherRigidBody) 
                continue;
            
            for (int i = 0; i < targetRigidBody->collisionMesh->vertices.size(); i+=3) {
                auto targetVertices = targetRigidBody->collisionMesh->vertices;
                auto targetTranform = targetRigidBody->transform;

                Triangle targetTriangle = {
                    (targetVertices[i+0] + targetTranform->position) * targetTranform->scale,
                    (targetVertices[i+1] + targetTranform->position) * targetTranform->scale,
                    (targetVertices[i+2] + targetTranform->position) * targetTranform->scale
                };

                for (int j = 0; j < otherRigidBody->collisionMesh->vertices.size(); j+=3) {
                    auto otherVertices = otherRigidBody->collisionMesh->vertices; 
                    auto otherTransform = otherRigidBody->transform;

                    Triangle otherTriangle = {
                        (otherVertices[j+0] + otherTransform->position) * otherTransform->scale,
                        (otherVertices[j+1] + otherTransform->position) * otherTransform->scale,
                        (otherVertices[j+2] + otherTransform->position) * otherTransform->scale
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