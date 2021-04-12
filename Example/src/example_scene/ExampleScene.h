#pragma once

#include <UniversoEngine.h>

using namespace engine;

class ExampleScene : public Scene {

private:
	CameraController* cameraInput;

	Texture* boxTexture;
	Mesh* boxMesh;

public:

	~ExampleScene();

private:

	void onStart() override;

	void onUpdate(float deltaTime) override;
};