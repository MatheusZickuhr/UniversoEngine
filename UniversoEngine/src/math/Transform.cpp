#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace engine {

	glm::mat4 Transform::getTransformMatrix() {
		return glm::translate(glm::mat4(1.0f), this->position)
			* glm::toMat4(glm::quat(this->rotation))
			* glm::scale(glm::mat4(1.0f), this->scale);;
	}

}