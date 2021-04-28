#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "renderer_api/FrameBuffer.h"
#include "renderer_api/Texture.h"

namespace engine {

	struct DirectionalLight {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

		std::shared_ptr<FrameBuffer> depthMapFrameBuffer = std::make_shared<FrameBuffer>();
		std::shared_ptr<Texture> depthMapTexture = std::make_shared<Texture>(1024.0f, 1024.0f);


		DirectionalLight() {
			this->depthMapFrameBuffer->addTextureAsDepthBuffer(*this->depthMapTexture.get());
		}
		
		glm::mat4 getViewProjectionMatrix() {
			float nearPlane = 1.0f, farPlane = 7.5f;
			glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
			glm::mat4 lightView = glm::lookAt(this->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 lightSpaceMatrix = lightProjection * lightView;

			return lightSpaceMatrix;
		}
	};

	struct PointLight {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;
	};

}

