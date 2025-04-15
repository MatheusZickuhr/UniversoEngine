#pragma once
#include <UniversoEngine.h>

using namespace engine;

class PongScene : public Scene {

private:

	std::unique_ptr<Camera3dController> cameraController;

	Entity sprite;

	void onStart() override;

	void onUpdate(float deltaTime) override;
};

