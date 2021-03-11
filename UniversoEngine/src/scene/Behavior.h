#pragma once
#include "Entity.h"

namespace engine {

    class Behavior {
    private:
        bool initialized;
    protected:
        Entity* entity;

    public:
        Behavior(Entity* entity) : entity(entity), initialized(false) {}
        
        void initialize();

        bool isInitialized();

        virtual void onUpdate(float deltaTime) = 0;

        virtual void onStart() = 0; 
    };
}