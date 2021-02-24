#include "RigidBody.h"

namespace engine {

    RigidBody::RigidBody(glm::vec3 position): position(position) {
        this->velocity = {0.0f, 0.0f, 0.0f};       
        this->accelaration = {0.0f, 0.0f, 0.0f};
        this->mass = 1.0f;       
        this->isStatic = false;
    }

    void RigidBody::applyForce(glm::vec3 force) {
        this->accelaration = force / mass;
    }

    void RigidBody::updatePosition(float deltaTime) {
        this->velocity += this->accelaration;
        this->position += this->velocity * deltaTime;
        this->velocity = {0.0f, 0.0f, 0.0f}; 
    }

}
