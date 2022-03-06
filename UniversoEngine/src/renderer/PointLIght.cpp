#include "PointLight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	PointLight::PointLight() : 
		depthFrameBuffer(std::make_shared<FrameBuffer>()),
		depthCubeMap(CubeMap::createDepthCubeMap(2048, 2048)) {

		depthFrameBuffer->addDepthCubeMap(depthCubeMap);
	}

	std::array<glm::mat4, 6> PointLight::getViewShadowMatrices() {
		float aspectRatio = (float) depthCubeMap->getWidth() / (float) depthCubeMap->getHeight();
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspectRatio, nearPlane, farPlane);

		std::array<glm::mat4, 6> shadowTransforms;

		shadowTransforms[0] = shadowProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[1] = shadowProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[2] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
		shadowTransforms[3] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
		shadowTransforms[4] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
		shadowTransforms[5] = shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

		return shadowTransforms;
	}

	std::shared_ptr<FrameBuffer> PointLight::getDepthFrameBuffer() {
		return depthFrameBuffer;
	}

	std::shared_ptr<CubeMap> PointLight::getDepthCubeMap() const {
		return depthCubeMap;
	}

}
