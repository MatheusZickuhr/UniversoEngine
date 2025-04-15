#include "Camera2d.h"

namespace engine {

	glm::mat4 Camera2d::getViewMatrix() {
		return glm::mat4(1.0f);
	}

	glm::mat4 Camera2d::getProjectionMatrix(float width, float height) {
		float aspectRatio = width / height;
		return glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
	}

	glm::mat4 Camera2d::getViewProjectionMatrix(float width, float height) {
		return getProjectionMatrix(width, height) * getViewMatrix();
	}

	glm::vec3 Camera2d::getPosition() {
		return position;
	}
}