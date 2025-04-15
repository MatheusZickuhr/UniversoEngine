#pragma once

#include "Camera.h"

namespace engine {

	class Camera2d : public Camera {

	public:

		glm::mat4 getViewMatrix() override;

		glm::mat4 getProjectionMatrix(float width, float height) override;

		glm::mat4 getViewProjectionMatrix(float width, float height) override;

		glm::vec3 getPosition() override;

	private:

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };

	};

}
