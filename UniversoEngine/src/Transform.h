#pragma once
#include <glm/glm.hpp>

namespace engine {

	class Transform {

	public:
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotationAxis;
		float rotationAngle;

		Transform();
	};
}
