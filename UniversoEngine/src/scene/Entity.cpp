#include "Entity.h"
#include "Components.h"
#include "../debug/Assert.h"

namespace engine {

    template<>
    BehaviorComponent& Entity::addComponent<BehaviorComponent>() {

        return scene->getRegistry().emplace<BehaviorComponent>(entity, this);
    }
}