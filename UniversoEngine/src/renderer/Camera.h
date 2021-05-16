#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*camera implementation based on https://learnopengl.com/Getting-started/Camera */

namespace engine {
	
    class Camera {

    public:

        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 up       = { 0.0f, 0.0f, 0.0f };
        glm::vec3 right    = { 0.0f, 0.0f, 0.0f };
        glm::vec3 front    = { 0.0f, 0.0f, -1.0f };
        glm::vec3 worldUp  = { 0.0f, 1.0f, 0.0f };

        // euler Angles
        float yaw   = -90.0f;
        float pitch = 0.0f;

        float fov = 45.0f;

        Camera();

        Camera(glm::vec3 position);


        glm::mat4 getViewProjectionMatrix(float width, float height);

    private:

        void updateVectors();

    };
}
