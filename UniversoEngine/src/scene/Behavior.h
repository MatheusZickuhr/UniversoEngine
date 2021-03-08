#pragma once
#include "Entity.h"

namespace engine {

    class Behavior {
        
    private:
        Entity* entity;

    public:
        Behavior(Entity* entity) : entity(entity) {}
        
        virtual void onUpdate(float deltaTime) = 0;

        virtual void onStart() = 0; 
    };
}