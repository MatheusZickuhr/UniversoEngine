#include "scene/GameObject.h"

class Obstacle : public engine::GameObject {

    using engine::GameObject::GameObject;

    void onUpdate(float deltaTime) override;
    void onStart() override;
};