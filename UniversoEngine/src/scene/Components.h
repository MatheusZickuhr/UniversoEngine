#pragma once

#include <vector>

#include "../physics/RigidBody.h"
#include "../physics/CollisionBody.h"

#include "../renderer/renderer_api/Texture.h"
#include "../renderer/Material.h"
#include "../renderer/Mesh.h"
#include "../renderer/DirectionalLight.h"
#include "../renderer/PointLight.h"

#include "../math/Transform.h"

#include "Entity.h"
#include "Behavior.h"

namespace engine {

    struct PointLightComponent {
        PointLight pointLight;
    };

    struct DirectionalLightComponent {
        DirectionalLight directionalLight;
    };

    struct DynamicMeshComponent {
        Mesh* mesh;
    };

    struct StaticMeshComponent {
        Mesh* mesh;
    };

    struct MaterialComponent {
        Material* material;
    };

    struct CollisionShapeComponent {
        CollisionShape collisionShape;
    };

    struct RigidBodyComponent {
        RigidBodyType type = RigidBodyType::Dynamic;
        std::shared_ptr<RigidBody> rigidBody;
    };

    struct CollisionBodyComponent {
        std::shared_ptr<CollisionBody> collisionBody;
    };

    struct TransformComponent {
        Transform transform;
    };

    struct BehaviorComponent {
        Entity entity;
        Scene* scene;
        Behavior* behavior;

        template<typename T>
        void bindBehavior() {
            behavior = new T(entity, scene);
        }

        template<typename T>
        T* getBehaviorInstance() {
            return static_cast<T*>(behavior);
        }


        // free the allocated memory
        void destroy() {
            delete this->behavior;
        }
    };

    struct NameComponent {
        std::string name;
    };

}