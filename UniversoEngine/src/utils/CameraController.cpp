#include "CameraController.h"


namespace engine {

	CameraController::CameraController(Camera& camera) :
		camera(camera),
		movementSpeed(SPEED),
		mouseSensitivity(SENSITIVITY) {
		
	}

	void CameraController::update(float deltaTime) {
		this->processKeybordInput(deltaTime);
		this->processMouseInput();
	}

	void CameraController::processKeybordInput(float deltaTime) {
		if (engine::Input::keyPressed('W'))
			this->updateCameraPosition(CameraMovement::FORWARD, deltaTime);
		if (engine::Input::keyPressed('S'))
			this->updateCameraPosition(CameraMovement::BACKWARD, deltaTime);
		if (engine::Input::keyPressed('A'))
			this->updateCameraPosition(CameraMovement::LEFT, deltaTime);
		if (engine::Input::keyPressed('D'))
			this->updateCameraPosition(CameraMovement::RIGHT, deltaTime);
	}

	void CameraController::processMouseInput() {
		double xpos, ypos;
		engine::Input::getCursorPos(&xpos, &ypos);

		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		this->updateCameraAngles(xoffset, yoffset);
	}

	void CameraController::updateCameraPosition(CameraMovement direction, float deltaTime) {
		float velocity = this->movementSpeed * deltaTime;
		if (direction == CameraMovement::FORWARD)
			this->camera.position += this->camera.front * velocity;

		if (direction == CameraMovement::BACKWARD)
			this->camera.position -= this->camera.front * velocity;

		if (direction == CameraMovement::LEFT)
			this->camera.position -= this->camera.right * velocity;

		if (direction == CameraMovement::RIGHT)
			this->camera.position += this->camera.right * velocity;

	}

	void CameraController::updateCameraAngles(float xoffset, float yoffset, bool constrainPitch) {
		xoffset *= this->mouseSensitivity;
		yoffset *= this->mouseSensitivity;

		this->camera.yaw += xoffset;
		this->camera.pitch += yoffset;

		if (constrainPitch) {
			if (this->camera.pitch > 89.0f)
				this->camera.pitch = 89.0f;
			if (this->camera.pitch < -89.0f)
				this->camera.pitch = -89.0f;
		}

		this->camera.updateVectors();
	}
}