#include "CameraController.h"
#include "../Application.h"
#include "../Window.h"

namespace engine {

	CameraController::CameraController(std::shared_ptr<Window> window, Camera& camera) : window(window), camera(camera) {}

	void CameraController::update(float deltaTime) {
		this->processKeybordInput(deltaTime);
		this->processMouseInput();
	}

	void CameraController::processKeybordInput(float deltaTime) {
		float velocity = this->movementSpeed * deltaTime;

		
		if (window->keyPressed('W'))
			this->camera.position += this->camera.front * velocity;

		if (window->keyPressed('S'))
			this->camera.position -= this->camera.front * velocity;

		if (window->keyPressed('A'))
			this->camera.position -= this->camera.right * velocity;

		if (window->keyPressed('D'))
			this->camera.position += this->camera.right * velocity;

	}

	void CameraController::processMouseInput() {

		float xcurrent, ycurrent;
		window->getCursorPos(&xcurrent, &ycurrent);

		if (isFirstMouseMovement) {
			xlast = xcurrent;
			ylast = ycurrent;
			isFirstMouseMovement = false;
		}

		float xoffset = xcurrent - xlast;
		float yoffset = ylast - ycurrent;

		xlast = xcurrent;
		ylast = ycurrent;

		xoffset *= this->mouseSensitivity;
		yoffset *= this->mouseSensitivity;

		this->camera.yaw += xoffset;
		this->camera.pitch += yoffset;

		this->camera.pitch = glm::clamp(this->camera.pitch, -89.0f, 89.0f);
	}

}