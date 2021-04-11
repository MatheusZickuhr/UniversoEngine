#pragma once

#include <memory>
#include "../renderer/Camera.h"
#include "../input/Input.h"

namespace engine {

	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;

	enum class CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class CameraController {

	public:

		CameraController(Camera& camera);

		void update(float deltaTime);

	private:

		Camera& camera;
		float movementSpeed;
		float mouseSensitivity;
		float lastX = 0;
		float lastY = 0;
		bool firstMouse = true;

		void processKeybordInput(float deltaTime);

		void processMouseInput();

		void updateCameraPosition(CameraMovement direction, float deltaTime);

		void updateCameraAngles(float xoffset, float yoffset, bool constrainPitch = true);
	};
}