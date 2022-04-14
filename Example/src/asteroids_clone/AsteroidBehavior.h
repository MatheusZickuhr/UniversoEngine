#pragma once

#include <UniversoEngine.h>

using namespace engine;

class AsteroidBehavior : public Behavior {

public:

    using Behavior::Behavior;

    void onStart() override;

    void onUpdate(float deltaTime) override;

    void setDirection(glm::vec2 direction) { this->direction = direction; }

    void onCollision(Entity other) override;

private:

    glm::vec2 direction = { 0.0f, 0.0f };

    float speed = 2.0f;

    float currentLifeTime = 0.0f;
    const float maxLifeTime = 10.0f;

    Random random{};
};