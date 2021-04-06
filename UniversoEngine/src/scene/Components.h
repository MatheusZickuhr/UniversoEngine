#pragma once

#include <memory>
#include <vector>

#include "../physics/RigidBody.h"

#include "../renderer/renderer_api/Texture.h"
#include "../renderer/Mesh.h"

#include "../math/Transform.h"

#include "Entity.h"
#include "Behavior.h"

namespace engine {

   
    struct MeshComponent {
        Mesh* mesh;
    };

    struct TextureComponent {
        Texture* texture;
    };

    struct CollisionShapeComponent {
        CollisionShape collisionShape;
    };

    struct RigidBodyComponent {
        RigidBodyType type = RigidBodyType::Dynamic;
        RigidBody rigidBody;
    };

    struct TransformComponent {
        Transform transform;
    };

    struct BehaviorComponent {
        std::shared_ptr<Behavior> behavior;
        Entity* entity = nullptr;

        BehaviorComponent(Entity* entity) {
            this->entity = entity;
        }
    
        template<typename T>
        void bindBehavior() {
            behavior = std::make_shared<T>(entity);
        }
    };

}