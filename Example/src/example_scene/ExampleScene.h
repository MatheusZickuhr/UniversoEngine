#pragma once
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "utils/CameraController.h"
#include "renderer/Mesh.h"
#include "renderer/renderer_api/Texture.h"
#include "CrateBehavior.h"
#include "math/Random.h"

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