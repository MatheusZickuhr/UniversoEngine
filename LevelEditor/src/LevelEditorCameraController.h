#pragma once

#include <memory>
#include <UniversoEngine.h>

using namespace engine;

class LevelEditorCameraController {

public:

	LevelEditorCameraController(Camera& camera);

	void update(float deltaTime);

private:

	Camera& camera;
	float movementSpeed = 2.5f;
	float mouseSensitivity = 0.1f;
	float xlast = 0;
	float ylast = 0;
	bool isFirstMouseMovement = true;

	void processKeybordInput(float deltaTime);

	void processMouseInput();

};
