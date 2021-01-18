#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

/*camera implementation based on https://learnopengl.com/Getting-started/Camera */

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

        this->updateCameraVectors();
    }

    void updateCameraVectors() {
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

    float getZoom() {
        return this->zoom;
    }

    void setZoom(float zoom ) {
        this->zoom = zoom;
    }

    float getPitch() {
        return this->pitch;
    }

    void setPitch(float pitch) {
        this->pitch = pitch;
    }

    float getYaw() {
        return this->yaw;
    }

    void setYaw(float yaw) {
        this->yaw = yaw;
    }

    glm::vec3 getFront() {
        return this->front;
    }

    glm::vec3 getRight() {
        return this->right;
    }

    glm::vec3 getPosition() {
        return this->position;
    }

    void setPosition(glm::vec3 position) {
        this->position = position;
    }

};
