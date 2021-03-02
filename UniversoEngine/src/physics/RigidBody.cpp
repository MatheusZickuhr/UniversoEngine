#include "RigidBody.h"

namespace engine {

    RigidBody::RigidBody(std::shared_ptr<Transform> transform, std::shared_ptr<CollisionMesh> collisionMesh)
    : transform(transform), collisionMesh(collisionMesh) {
        this->velocity = {0.0f, 0.0f, 0.0f};       
        this->accelaration = {0.0f, 0.0f, 0.0f};
        this->mass = 1.0f;       
        this->isStatic = false;
    }

    void RigidBody::applyForce(glm::vec3 force) {
        this->accelaration += force / mass;
    }

    void RigidBody::update(float deltaTime) {
        this->velocity += this->accelaration;
        this->transform->position += this->velocity * deltaTime;
        
        this->accelaration = {0.0f, 0.0f, 0.0f}; 

        this->velocity = glm::clamp(velocity, minVelocity, maxVelocity);
    }

}
