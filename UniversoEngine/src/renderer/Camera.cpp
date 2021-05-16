#include "Camera.h"

namespace engine {

	Camera::Camera() {
		this->updateVectors();
	}

	Camera::Camera(glm::vec3 position) : position(position) {
		this->updateVectors();	
	}

	glm::mat4 Camera::getViewProjectionMatrix(float width, float height) {
		this->updateVectors();

		glm::mat4 projection = glm::perspective(glm::radians(this->fov), width / height, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(this->position, this->position + this->front, this->up);

		return projection * view;
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

}