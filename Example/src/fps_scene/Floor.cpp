#include "Floor.h"


	void Floor::onStart() {
        this->rigidBody->isStatic = true;
        
        float size = 20;
        this->transform->position.y = -5;
        this->transform->scale.x = size;
        this->transform->scale.z = size;
    }

	void Floor::onUpdate(float deltaTime) {}