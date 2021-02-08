#pragma once

namespace engine {

	class Transform {

	public:
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotationAxis;
		float rotationAngle;

		Transform() :
			position(glm::vec3(0.0f, 0.0f, 0.0f)),
			scale(glm::vec3(1.0f, 1.0f, 1.0f)),
			rotationAxis(glm::vec3(0.0f, 0.0f, 0.0f)),
			rotationAngle(0.0f) {
		}
	};
}
