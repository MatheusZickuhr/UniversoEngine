#include "DebugCameraController.h"


namespace engine {

	DebugCameraController::DebugCameraController(std::shared_ptr<engine::Camera> camera) : movementSpeed(SPEED), mouseSensitivity(SENSITIVITY) {
		this->camera = camera;
	}

	void DebugCameraController::update(float deltaTime) {
		this->processKeybordInput(deltaTime);
		this->processMouseInput();
	}

	void DebugCameraController::processKeybordInput(float deltaTime) {
		if (engine::Input::keyPressed('W'))
			this->updateCameraPosition(FORWARD, deltaTime);
		if (engine::Input::keyPressed('S'))
			this->updateCameraPosition(BACKWARD, deltaTime);
		if (engine::Input::keyPressed('A'))
			this->updateCameraPosition(LEFT, deltaTime);
		if (engine::Input::keyPressed('D'))
			this->updateCameraPosition(RIGHT, deltaTime);
	}

	void DebugCameraController::processMouseInput() {
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

	void DebugCameraController::updateCameraPosition(CameraMovement direction, float deltaTime) {
		float velocity = this->movementSpeed * deltaTime;
		if (direction == FORWARD)
			this->camera->position += this->camera->front * velocity;

		if (direction == BACKWARD)
			this->camera->position -= this->camera->front * velocity;

		if (direction == LEFT)
			this->camera->position -= this->camera->right * velocity;

		if (direction == RIGHT)
			this->camera->position += this->camera->right * velocity;

	}

	void DebugCameraController::updateCameraAngles(float xoffset, float yoffset, GLboolean constrainPitch) {
		xoffset *= this->mouseSensitivity;
		yoffset *= this->mouseSensitivity;

		this->camera->yaw += xoffset;
		this->camera->pitch += yoffset;

		if (constrainPitch) {
			if (this->camera->pitch > 89.0f)
				this->camera->pitch = 89.0f;
			if (this->camera->pitch < -89.0f)
				this->camera->pitch = -89.0f;
		}

		this->camera->updateVectors();
	}
}