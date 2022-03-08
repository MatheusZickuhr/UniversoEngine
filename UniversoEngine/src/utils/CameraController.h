#pragma once

#include <memory>
#include "../renderer/Camera.h"
#include "../input/Input.h"

namespace engine {

	class CameraController {

	public:

		CameraController(Camera& camera);

		void update(float deltaTime);

	private:

		Camera& camera;
		float movementSpeed = 20.0f;
		float mouseSensitivity = 0.1f;
		float xlast = 0;
		float ylast = 0;
		bool isFirstMouseMovement = true;

		void processKeybordInput(float deltaTime);

		void processMouseInput();

	};
}