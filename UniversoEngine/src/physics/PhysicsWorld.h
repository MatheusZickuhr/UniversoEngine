#include <vector>
#include <glm/glm.hpp>
#include "RigidBody.h"

namespace engine {

    class PhysicsWorld {

    private:
        const glm::vec3 gravityForce = {0.0f, -1.0f, 0.0f};
        std::vector<RigidBody*> rigidBodies;        

    public:
        void update(float deltaTime);

        void appendRigidBody(RigidBody* rigidBody);

        void clear();
    };
}