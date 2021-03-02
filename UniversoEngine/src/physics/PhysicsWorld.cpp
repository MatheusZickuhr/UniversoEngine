#include "PhysicsWorld.h"

namespace engine {

    void PhysicsWorld::update(float deltaTime) {

        for (auto rigidBody : this->rigidBodies) {
            if (rigidBody->isStatic) 
                continue;

            Collision collision = this->checkCollision(rigidBody);
            
            if (collision.happened) {
                rigidBody->velocity = {0.0f, 0.0f, 0.0f};
                rigidBody->applyForce(-gravityForce);
            }

            rigidBody->applyForce(gravityForce);

            rigidBody->update(deltaTime);
        }
    }

    Collision PhysicsWorld::checkCollision(std::shared_ptr<RigidBody> targetRigidBody) {

        for (auto otherRigidBody: this->rigidBodies) {

            if (targetRigidBody == otherRigidBody) 
                continue;
            
            for (int i = 0; i < targetRigidBody->collisionMesh->vertices.size(); i+=3) {
                auto targetVertices = targetRigidBody->collisionMesh->vertices;
                auto targetTranform = targetRigidBody->transform;

                CollisionCheckTriangle targetTriangle = {
                    (targetVertices[i+0] + targetTranform->position) * targetTranform->scale,
                    (targetVertices[i+1] + targetTranform->position) * targetTranform->scale,
                    (targetVertices[i+2] + targetTranform->position) * targetTranform->scale
                };

                for (int j = 0; j < otherRigidBody->collisionMesh->vertices.size(); j+=3) {
                    auto otherVertices = otherRigidBody->collisionMesh->vertices; 
                    auto otherTransform = otherRigidBody->transform;

                    CollisionCheckTriangle otherTriangle = {
                        (otherVertices[j+0] + otherTransform->position) * otherTransform->scale,
                        (otherVertices[j+1] + otherTransform->position) * otherTransform->scale,
                        (otherVertices[j+2] + otherTransform->position) * otherTransform->scale
                    };

                    auto doesCollid = trianglesIntersects(&targetTriangle, &otherTriangle);

                    if (doesCollid) {
                        glm::vec3 reverseDirection = glm::normalize(
                            targetTriangle.centerPosition() - otherTriangle.centerPosition()
                        );
                        return {true, reverseDirection};
                    }
                        
                }
            }
        }

        return {false};
    }

    void PhysicsWorld::appendRigidBody(std::shared_ptr<RigidBody> rigidBody) {
        this->rigidBodies.push_back(rigidBody);
    }

    void PhysicsWorld::clear() {
        this->rigidBodies.clear();
    }
}