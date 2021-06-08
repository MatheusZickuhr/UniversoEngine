#pragma once

#include <UniversoEngine.h>
#include "LevelEditorCameraController.h"

using namespace engine;

class DefaultLevelEditorScene : public Scene {

private:
	LevelEditorCameraController* cameraController;

	Material boxMaterial{ "Example/resources/textures/crate/crate.jpg" };
	Mesh boxMesh{ "Example/resources/models/crate/crate.obj" };

	CubeMap skyboxCubeMap {
		{
			"Example/resources/textures/exampleSkyBox/right.jpg",
			"Example/resources/textures/exampleSkyBox/left.jpg",
			"Example/resources/textures/exampleSkyBox/top.jpg",
			"Example/resources/textures/exampleSkyBox/bottom.jpg",
			"Example/resources/textures/exampleSkyBox/front.jpg",
			"Example/resources/textures/exampleSkyBox/back.jpg"
		}
	};

public:

	~DefaultLevelEditorScene();

private:

	void onStart() override;

	void onUpdate(float deltaTime) override;
};