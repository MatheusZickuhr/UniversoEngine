#pragma once
#include <memory>
#include "../physics/RigidBody.h"


namespace engine {
    struct RigidBodyComponent {
        std::shared_ptr<RigidBody> rigidBody;
    };

}
