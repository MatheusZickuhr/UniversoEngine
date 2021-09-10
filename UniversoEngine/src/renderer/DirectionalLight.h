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
		glm::vec3 lookAt = { 0.0f, 0.0f, 0.0f };
		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

		DirectionalLight();

		glm::mat4 getViewProjectionMatrix();

		FrameBuffer* getDepthBufferFrameBuffer();

		DepthBufferTexture2D* getDepthBufferTexture();

	private:

		std::shared_ptr<FrameBuffer> depthBufferFrameBuffer;
		std::shared_ptr<DepthBufferTexture2D> depthBufferTexture;

	};

}