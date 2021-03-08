#include "Entity.h"

namespace engine {

    class Behavior {
        
    private:
        Entity* entity;

    public:
        Behavior(Entity* entity) : entity(entity) {}
        
        virtual void onUpdate(float deltaTime);

        virtual void onStart(); 
    };
}