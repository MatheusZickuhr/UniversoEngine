#pragma once

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
        RigidBody* rigidBody;
    };

    struct TransformComponent {
        Transform transform;
    };

    struct BehaviorComponent {
        Entity* entity;
        Behavior* behavior;

        template<typename T>
        void bindBehavior() {
            behavior = new T(entity);
        }

        // free the allocated memory
        void destroy() {
            delete this->behavior;
        }
    };

}