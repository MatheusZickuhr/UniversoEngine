#pragma once

#include <memory>
#include <vector>

#include "../renderer/renderer_api/Texture.h"
#include "../renderer/Mesh.h"

#include "../physics/RigidBody.h"
#include "../math/Transform.h"

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

}