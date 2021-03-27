#include "RigidBody.h"
#include "tri_tri_intersect/tri_tri_intersect.h"

namespace engine {

    RigidBody::RigidBody(std::vector<glm::vec3> collisionMesh, Transform* transform)
    : transform(transform), collisionMesh(collisionMesh) {
        this->velocity = {0.0f, 0.0f, 0.0f};       
        this->acceleration = {0.0f, 0.0f, 0.0f};
        this->mass = 1.0f;       
        this->isStatic = false;
    }

    void RigidBody::addForce(glm::vec3 force) {
        this->toBeAppliedForces.push_back(force / mass); 
    }

    void RigidBody::applyForce(glm::vec3 force) {
        this->acceleration += force;
    }

    void RigidBody::update() {
        this->velocity += this->acceleration;
        this->transform->position += this->velocity;
        this->acceleration = {0.0f, 0.0f, 0.0f};
        this->velocity = {0.0f, 0.0f, 0.0f};
        this->velocity = glm::clamp(velocity, minVelocity, maxVelocity);
    }

    void RigidBody::moveToNextState() {
        this->prevVelocity = this->velocity;
        this->prevPosition = this->transform->position;

        this->velocity += this->acceleration;
        this->transform->position += this->velocity;
    }

    void RigidBody::moveToPrevState() {
        this->velocity = this->prevVelocity;
        this->transform->position = this->prevPosition;
    }

    bool RigidBody::collidesWith(RigidBody* other) {
        for (int i = 0; i < collisionMesh.size(); i+=3) {     
            auto v0 = (collisionMesh[i+0] + this->transform->position) * this->transform->scale;
            auto v1 = (collisionMesh[i+1] + this->transform->position) * this->transform->scale;
            auto v2 = (collisionMesh[i+2] + this->transform->position) * this->transform->scale;

            for (int j = 0; j < other->collisionMesh.size(); j+=3) {
                auto u0 = (other->collisionMesh[j+0] + other->transform->position) * other->transform->scale;
                auto u1 = (other->collisionMesh[j+1] + other->transform->position) * other->transform->scale;
                auto u2 = (other->collisionMesh[j+2] + other->transform->position) * other->transform->scale;

                auto doesCollid = tri_tri_intersect(
                    glm::value_ptr(v0), glm::value_ptr(v1), glm::value_ptr(v2),
                    glm::value_ptr(u0), glm::value_ptr(u1), glm::value_ptr(u2));

                if (doesCollid) {
                    return true;
                }   
            }
            
        }
        return false;
    }
}
