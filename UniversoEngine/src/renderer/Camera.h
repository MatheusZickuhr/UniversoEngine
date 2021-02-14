#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*camera implementation based on https://learnopengl.com/Getting-started/Camera */

namespace engine {
	
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float ZOOM = 45.0f;

    class Camera {
    public:

        // camera Attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        // euler Angles
        float yaw;
        float pitch;

        // camera options
        float zoom;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),float yaw = YAW,float pitch = PITCH);

        void updateVectors();

        glm::mat4 getViewMatrix();

        glm::mat4 getMvp(float width, float height);
    };
}
