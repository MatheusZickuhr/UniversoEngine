#include "PointLight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	PointLight::PointLight() {
		this->depthMapFrameBuffer->addDepthBufferCubeMap(*this->depthMapCubeMap.get());
	}

	std::array<glm::mat4, 6> PointLight::getViewShadowMatrices() {
		float aspectRatio = depthMapCubeMap->getWidth() / depthMapCubeMap->getHeight();
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspectRatio, this->nearPlane, this->farPlane);

		std::array<glm::mat4, 6> shadowTransforms;

		shadowTransforms[0] = shadowProj * glm::lookAt(this->position, this->position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[1] = shadowProj * glm::lookAt(this->position, this->position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[2] = shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
		shadowTransforms[3] = shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
		shadowTransforms[4] = shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[5] = shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

		return shadowTransforms;
	}

}
