#pragma once

#include <memory>
#include "../renderer/renderer_api/Texture.h"
namespace engine {

    struct TextureComponent {

        std::shared_ptr<Texture> texture;
    };

}
