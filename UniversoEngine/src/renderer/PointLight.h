#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "renderer_api/FrameBuffer.h"
#include "renderer_api/DepthBufferCubeMap.h"


namespace engine {

	class PointLight {
		
	public:

		const static int maxPointLights = 4;

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

		PointLight();

		std::vector<glm::mat4> getViewProjectionMatrices();
	};

}
