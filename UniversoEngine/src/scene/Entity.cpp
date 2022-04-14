#include "Entity.h"
#include "Components.h"

namespace engine {

    template<>
    BehaviorComponent& Entity::addComponent<BehaviorComponent>() {
        ASSERT(isValid(), "Entity must be valid (It was probably destroyed or not initialized)");

        return scene->getRegistry().emplace<BehaviorComponent>(enttEntity, *this, scene);
    }
}