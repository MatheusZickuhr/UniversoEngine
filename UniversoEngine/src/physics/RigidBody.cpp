#include "RigidBody.h"
#include "tri_tri_intersect/tri_tri_intersect.h"

namespace engine {

    RigidBody::RigidBody(std::shared_ptr<Transform> transform, std::shared_ptr<CollisionMesh> collisionMesh)
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

    bool RigidBody::collidesWith(std::shared_ptr<RigidBody> other) {
        for (int i = 0; i < this->collisionMesh->vertices.size(); i+=3) {
            auto vertices = this->collisionMesh->vertices;
            
            auto v0 = (vertices[i+0] + this->transform->position) * this->transform->scale;
            auto v1 = (vertices[i+1] + this->transform->position) * this->transform->scale;
            auto v2 = (vertices[i+2] + this->transform->position) * this->transform->scale;

            for (int j = 0; j < other->collisionMesh->vertices.size(); j+=3) {
                auto otherVertices = other->collisionMesh->vertices; 
                auto otherTransform = other->transform;

                auto u0 = (otherVertices[j+0] + otherTransform->position) * otherTransform->scale;
                auto u1 = (otherVertices[j+1] + otherTransform->position) * otherTransform->scale;
                auto u2 = (otherVertices[j+2] + otherTransform->position) * otherTransform->scale;

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
