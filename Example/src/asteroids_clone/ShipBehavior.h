#pragma once

#include <UniversoEngine.h>

using namespace engine;

class ShipBehavior : public Behavior {

public:

    using Behavior::Behavior;

    void onStart() override;

    void onUpdate(float deltaTime) override;

private:

    void handleRotationMovement(float deltaTime, TransformComponent& playerTransformComponent);
    
    void handleMovement(float deltaTime, TransformComponent& playerTransformComponent);
   
};