#pragma once

#include <UniversoEngine.h>

using namespace engine;

class ExampleScene : public Scene {

private:
	CameraController* cameraInput;

	Material* boxMaterial;
	Mesh* boxMesh;

	Material* redMaterial;

public:

	~ExampleScene();

private:

	void onStart() override;

	void onUpdate(float deltaTime) override;
};