#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	DirectionalLight::DirectionalLight() :
		depthBufferFrameBuffer(std::make_shared<FrameBuffer>()),
		depthBufferTexture(std::make_shared<DepthBufferTexture2D>(2048.0f, 2048.0f)) {

		depthBufferFrameBuffer->addDepthBufferTexture(*depthBufferTexture.get());
	}

	glm::mat4 DirectionalLight::getViewProjectionMatrix() {
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 7.5f);
		glm::mat4 lightView = glm::lookAt(this->position, this->lookAt, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		return lightSpaceMatrix;
	}

	FrameBuffer* DirectionalLight::getDepthBufferFrameBuffer() {
		return depthBufferFrameBuffer.get();
	}

	DepthBufferTexture2D* DirectionalLight::getDepthBufferTexture() {
		return depthBufferTexture.get();
	}

}
