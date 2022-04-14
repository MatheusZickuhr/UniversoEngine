#pragma once
#include "Entity.h"
#include "Scene.h"

namespace engine {

    class Behavior {
    private:
        bool initialized;
    protected:
        Entity entity;
        Scene* scene;


    public:
        Behavior(Entity entity, Scene* scene) : entity(entity), scene(scene), initialized(false) {}
        
        void initialize();

        bool isInitialized();

        virtual void onCollision(Entity other) {}

        virtual void onUpdate(float deltaTime) = 0;

        virtual void onStart() = 0; 
    };
}