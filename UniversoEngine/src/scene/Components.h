#pragma once

#include <memory>
#include <vector>

#include "../renderer/renderer_api/Texture.h"
#include "../renderer/Mesh.h"

#include "../physics/RigidBody.h"
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

    struct RigidBodyComponent {
        std::shared_ptr<RigidBody> rigidBody;

        RigidBodyComponent(std::vector<glm::vec3> collisionMesh) {
            rigidBody = std::make_shared<RigidBody>(collisionMesh);
        }
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