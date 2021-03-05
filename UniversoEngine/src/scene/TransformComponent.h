#pragma once

#include <memory>
#include "../math/Transform.h"

namespace engine {

    struct TransformComponent {

        std::shared_ptr<Transform> transform;
    };
}
