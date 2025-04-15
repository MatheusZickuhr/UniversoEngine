#pragma once

#include <memory>
#include "../renderer/Camera3d.h"
#include "../Window.h"

namespace engine {

	class Camera3dController {

	public:

		Camera3dController(std::shared_ptr<Window> window, std::shared_ptr<Camera3d> camera);

		void update(float deltaTime);

	private:
		std::shared_ptr<Window> window;
		std::shared_ptr<Camera3d> camera;
		float movementSpeed = 20.0f;
		float mouseSensitivity = 0.1f;
		float xlast = 0;
		float ylast = 0;
		bool isFirstMouseMovement = true;

		void processKeybordInput(float deltaTime);

		void processMouseInput();

	};
}