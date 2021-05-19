#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	DirectionalLight::DirectionalLight() {
		this->depthMapFrameBuffer->addDepthBufferTexture(*this->depthMapTexture.get());
	}

	glm::mat4 DirectionalLight::getViewProjectionMatrix() {
		float nearPlane = 1.0f, farPlane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
		glm::mat4 lightView = glm::lookAt(this->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		return lightSpaceMatrix;
	}

	DirectionalLight::Data DirectionalLight::getData() {
		Data directionalLightData;

		directionalLightData.position = { this->position, 0.0f };
		directionalLightData.ambient = { this->ambient, 0.0f };
		directionalLightData.diffuse = { this->diffuse, 0.0f };
		directionalLightData.specular = { this->specular, 0.0f };
		directionalLightData.textureSlotIndex = this->depthMapTexture->getSlot();
		directionalLightData.viewProjection = this->getViewProjectionMatrix();

		return directionalLightData;
	}

}
