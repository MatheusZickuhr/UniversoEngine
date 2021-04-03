#pragma once

#include <memory>
#include <vector>

#include <reactphysics3d/reactphysics3d.h>

#include "../renderer/renderer_api/Texture.h"
#include "../renderer/Mesh.h"

#include "../math/Transform.h"

#include "Entity.h"
#include "Behavior.h"

namespace engine {

    enum class CollisionShape { Box, Sphere, Capsule };

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
        bool isDynamic = true;
        reactphysics3d::RigidBody* rigidBody;

        void apllyForce(glm::vec3 force) {
            rigidBody->applyForceToCenterOfMass(reactphysics3d::Vector3(force.x, force.y, force.z));
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