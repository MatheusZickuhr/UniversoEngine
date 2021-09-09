#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	DirectionalLight::DirectionalLight() {
		this->depthMapFrameBuffer->addDepthBufferTexture(*this->depthMapTexture.get());
	}

	glm::mat4 DirectionalLight::getViewProjectionMatrix() {
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 7.5f);
		glm::mat4 lightView = glm::lookAt(this->position, this->lookAt, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		return lightSpaceMatrix;
	}

}
