#pragma once

#include "../renderer/Mesh.h"
#include <memory>
namespace engine {
    
    struct MeshComponent {

        std::shared_ptr<Mesh> mesh;
    };

}
