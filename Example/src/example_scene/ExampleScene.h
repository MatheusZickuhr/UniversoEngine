#pragma once

#include <UniversoEngine.h>

using namespace engine;

class ExampleScene : public Scene {

private:
	CameraController* cameraInput;

	Material boxMaterial{ "Example/resources/textures/crate/crate.jpg" };
	Mesh boxMesh{ "Example/resources/models/crate/crate.obj" };

public:

	~ExampleScene();

private:

	void onStart() override;

	void onUpdate(float deltaTime) override;
};