#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "renderer_api/FrameBuffer.h"
#include "renderer_api/DepthBufferTexture2D.h"
#include "renderer_api/DepthBufferCubeMap.h"

namespace engine {

	struct DirectionalLight {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

		std::shared_ptr<FrameBuffer> depthMapFrameBuffer = std::make_shared<FrameBuffer>();
		std::shared_ptr<DepthBufferTexture2D> depthMapTexture = std::make_shared<DepthBufferTexture2D>(1024.0f, 1024.0f);


		DirectionalLight() {
			this->depthMapFrameBuffer->addDepthBufferTexture(*this->depthMapTexture.get());
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
		float nearPlane = 1.0f;
		float farPlane = 25.0f;

		std::shared_ptr<FrameBuffer> depthMapFrameBuffer = std::make_shared<FrameBuffer>();
		std::shared_ptr<DepthBufferCubeMap> depthMapCubeMap = std::make_shared<DepthBufferCubeMap>(1024.0f, 1024.0f);

		PointLight() {
			this->depthMapFrameBuffer->addDepthBufferCubeMap(*this->depthMapCubeMap.get());
		}

		std::vector<glm::mat4> getViewProjectionMatrices() {
			float aspectRatio = 1.0f; // 1024 / 1024 for now
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
	};

}

