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
        
        RigidBody* rigidBody = nullptr;

        RigidBodyComponent(std::vector<glm::vec3> collisionMesh) {
            rigidBody = new RigidBody(collisionMesh);
        }
        
        ~RigidBodyComponent() {
            delete rigidBody;
        }
        
    };

    struct TransformComponent {
        Transform transform;
    };


    struct BehaviorComponent {
        Behavior* behavior = nullptr;
        Entity* entity = nullptr;

        BehaviorComponent(Entity* entity) {
            this->entity = entity;
        }
        
        ~BehaviorComponent() {
            delete behavior;
        }

        template<typename T>
        void bindBehavior() {
            behavior = new T(entity);
        }
    };

}