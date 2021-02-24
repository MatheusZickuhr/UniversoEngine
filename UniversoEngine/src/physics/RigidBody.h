#include <glm/glm.hpp>

namespace engine {

    class RigidBody {
    private:
        float mass;
        glm::vec3 accelaration;
        glm::vec3 velocity;

    public:
        glm::vec3 position;

        RigidBody(glm::vec3 position  = {0.0f, 0.0f, 0.0f});

        void applyForce(glm::vec3 force);

        void update(float deltaTime);    
    };
}