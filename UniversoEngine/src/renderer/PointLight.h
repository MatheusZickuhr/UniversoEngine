#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <array>

#include "renderer_api/FrameBuffer.h"
#include "renderer_api/DepthBufferCubeMap.h"


namespace engine {



	class PointLight {
		
	public:

		// std140 compatible struct
		struct alignas(16) Data {
			glm::vec4 position;
			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specular;
			float constant;
			float linear;
			float quadratic;
			float farPlane;
			int cubeMapSlotIndex;
		};

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
		std::shared_ptr<DepthBufferCubeMap> depthMapCubeMap = std::make_shared<DepthBufferCubeMap>(2048.0f, 2048.0f);

		PointLight();

		std::array<glm::mat4, 6> getViewShadowMatrices();

		Data getData();
	};

}
