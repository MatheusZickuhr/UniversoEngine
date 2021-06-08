#include "LevelEditorCameraController.h"

LevelEditorCameraController::LevelEditorCameraController(Camera& camera) : camera(camera) {}

void LevelEditorCameraController::update(float deltaTime) {
	if (Input::mouseButtonPressed(1)) {
		this->processKeybordInput(deltaTime);
		this->processMouseInput();
	}
	else {
		double xcurrent, ycurrent;
		Input::getCursorPos(&xcurrent, &ycurrent);
		xlast = xcurrent;
		ylast = ycurrent;
	}
}

void LevelEditorCameraController::processKeybordInput(float deltaTime) {
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

void LevelEditorCameraController::processMouseInput() {
	double xcurrent, ycurrent;
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

