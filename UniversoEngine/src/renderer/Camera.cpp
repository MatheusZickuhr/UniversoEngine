#include "Camera.h"

namespace engine {

	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
		this->front = glm::vec3(0.0f, 0.0f, -1.0f);
		this->zoom = ZOOM;
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;

		this->updateVectors();
	}

	void Camera::updateVectors() {
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		newFront.y = sin(glm::radians(this->pitch));
		newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(newFront);

		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

	glm::mat4 Camera::getViewMatrix() {
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	glm::mat4 Camera::getMvp(float width, float height) {
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(this->zoom), width / height, 0.1f, 100.0f);
		glm::mat4 view = this->getViewMatrix();

		return projection * view * model;
	}

}