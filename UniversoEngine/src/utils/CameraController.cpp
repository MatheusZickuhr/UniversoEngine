#include "CameraController.h"


namespace engine {

	CameraController::CameraController(Camera& camera) : camera(camera) {}

	void CameraController::update(float deltaTime) {
		this->processKeybordInput(deltaTime);
		this->processMouseInput();
	}

	void CameraController::processKeybordInput(float deltaTime) {
		float velocity = this->movementSpeed * deltaTime;

		if (Input::keyPressed('W'))
			this->camera.position += this->camera.front * velocity;

		if (Input::keyPressed('S'))
			this->camera.position -= this->camera.front * velocity;

		if (Input::keyPressed('A'))
			this->camera.position -= this->camera.right * velocity;

		if (Input::keyPressed('D'))
			this->camera.position += this->camera.right * velocity;

	}

	void CameraController::processMouseInput() {
		float xcurrent, ycurrent;
		Input::getCursorPos(&xcurrent, &ycurrent);

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