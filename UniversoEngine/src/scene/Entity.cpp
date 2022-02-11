#include "Entity.h"
#include "Components.h"

namespace engine {

    template<>
    BehaviorComponent& Entity::addComponent<BehaviorComponent>() {

        return scene->getRegistry().emplace<BehaviorComponent>(enttEntity, this);
    }
}