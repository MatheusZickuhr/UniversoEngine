#include "PointLight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	PointLight::PointLight() {
		this->depthMapFrameBuffer->addDepthBufferCubeMap(*this->depthMapCubeMap.get());
	}

	std::vector<glm::mat4> PointLight::getViewProjectionMatrices() {
		float aspectRatio = depthMapCubeMap->getWidth() / depthMapCubeMap->getHeight();
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspectRatio, this->nearPlane, this->farPlane);

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		return shadowTransforms;
	}
}