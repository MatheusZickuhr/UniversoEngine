#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

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

        Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW,
            float pitch = PITCH
        ) : front(glm::vec3(0.0f, 0.0f, -1.0f)), zoom(ZOOM) {
            this->position = position;
            this->worldUp = up;
            this->yaw = yaw;
            this->pitch = pitch;

            this->updateVectors();
        }

        void updateVectors() {
            glm::vec3 newFront;
            newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            newFront.y = sin(glm::radians(this->pitch));
            newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            this->front = glm::normalize(newFront);

            this->right = glm::normalize(glm::cross(this->front, this->worldUp));
            this->up = glm::normalize(glm::cross(this->right, this->front));
        }

        glm::mat4 getViewMatrix() {
            return glm::lookAt(this->position, this->position + this->front, this->up);
        }

        glm::mat4 getModelViewProjectionMatrix(float width, float height) {
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 projection = glm::perspective(glm::radians(this->zoom), width / height, 0.1f, 100.0f);
            glm::mat4 view = this->getViewMatrix();

            return projection * view * model;
        }
    };
}
