#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "renderer_api/FrameBuffer.h"
#include "renderer_api/DepthBufferTexture2D.h"

namespace engine {

	class  DirectionalLight {

	public:

		const static int maxDirectionalLights = 2;

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

		std::shared_ptr<FrameBuffer> depthMapFrameBuffer = std::make_shared<FrameBuffer>();
		std::shared_ptr<DepthBufferTexture2D> depthMapTexture = std::make_shared<DepthBufferTexture2D>(1024.0f, 1024.0f);


		DirectionalLight();

		glm::mat4 getViewProjectionMatrix();
	};

}