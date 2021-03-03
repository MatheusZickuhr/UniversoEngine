#include "Obstacle.h"

    void Obstacle::onStart() {
        this->transform->position = {3.0f, -3.0f, 3.0f};
        this->rigidBody->isStatic = true;
    }

    void Obstacle::onUpdate(float deltaTime) {}
