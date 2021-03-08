#include "Entity.h"

namespace engine {

    template<>
    RigidBodyComponent& Entity::addComponent<RigidBodyComponent>() {
        auto meshComp = getComponent<MeshComponent>();
        auto collisionMesh = meshComp.mesh->getVeticesPositions(); 
        return scene->registry.emplace<RigidBodyComponent>(entity, collisionMesh);
    }
}