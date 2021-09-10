#include "PointLight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	PointLight::PointLight() : 
		depthBufferFrameBuffer(std::make_shared<FrameBuffer>()),
		depthBufferCubeMap(std::make_shared<DepthBufferCubeMap>(2048.0f, 2048.0f)) {

		depthBufferFrameBuffer->addDepthBufferCubeMap(*depthBufferCubeMap.get());
	}

	std::array<glm::mat4, 6> PointLight::getViewShadowMatrices() {
		float aspectRatio = depthBufferCubeMap->getWidth() / depthBufferCubeMap->getHeight();
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

	FrameBuffer* PointLight::getDepthBufferFrameBuffer() {
		return depthBufferFrameBuffer.get();
	}

	DepthBufferCubeMap* PointLight::getDepthBufferCubeMap() {
		return depthBufferCubeMap.get();
	}

}
