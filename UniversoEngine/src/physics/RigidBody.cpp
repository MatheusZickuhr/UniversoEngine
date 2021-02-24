#include "RigidBody.h"

namespace engine {

    RigidBody::RigidBody(glm::vec3 position): position(position) {
        this->velocity = {0.0f, 0.0f, 0.0f};       
        this->accelaration = {0.0f, 0.0f, 0.0f};
        this->mass = 1.0f;       
    }

    void RigidBody::applyForce(glm::vec3 force) {
        this->accelaration = force / mass;
    }

    void RigidBody::update(float deltaTime) {
        this->velocity += accelaration;
        this->position += velocity * deltaTime;
    }

}
