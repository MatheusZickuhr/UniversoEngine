#include "Entity.h"
#include "Components.h"
#include "../debug/Assert.h"

namespace engine {

    template<>
    RigidBodyComponent& Entity::addComponent<RigidBodyComponent>() {
        ASSERT(hasComponent<MeshComponent>() && hasComponent<TransformComponent>(),
            "Entity must hava mesh and transform components to have a rigid body component");

        auto meshComp = getComponent<MeshComponent>();
        auto transComp = getComponent<TransformComponent>();

        auto transform = transComp.transform.get();
        auto collisionMesh = meshComp.mesh->getVeticesPositions(); 

        return scene->getRegistry().emplace<RigidBodyComponent>(entity, collisionMesh, transform);
    }

    template<>
    BehaviorComponent& Entity::addComponent<BehaviorComponent>() {

        return scene->getRegistry().emplace<BehaviorComponent>(entity, this);
    }
}