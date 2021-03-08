#include "Entity.h"
#include "Components.h"

namespace engine {

    template<>
    RigidBodyComponent& Entity::addComponent<RigidBodyComponent>() {
        auto meshComp = getComponent<MeshComponent>();
        auto collisionMesh = meshComp.mesh->getVeticesPositions(); 
        return scene->registry.emplace<RigidBodyComponent>(entity, collisionMesh);
    }

    template<>
    BehaviorComponent& Entity::addComponent<BehaviorComponent>() {

        return scene->registry.emplace<BehaviorComponent>(entity, this);
    }
}