#include "Camera.h"

namespace engine {

	Camera::Camera() {
		this->updateVectors();
	}

	Camera::Camera(glm::vec3 position) : position(position) {
		this->updateVectors();	
	}

	glm::mat4 Camera::getViewMatrix() {
		this->updateVectors();
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	glm::mat4 Camera::getProjectionMatrix(float width, float height) {
		return glm::perspective(glm::radians(this->fov), width / height, this->nearClippingDistance, this->farClippingDistance);
	}

	glm::mat4 Camera::getViewProjectionMatrix(float width, float height) {
		glm::mat4 projection = this->getProjectionMatrix(width, height);
		glm::mat4 view = this->getViewMatrix();

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