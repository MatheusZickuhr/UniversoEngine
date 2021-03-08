#pragma once
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "utils/DebugCameraController.h"
#include "renderer/Mesh.h"
#include "renderer/renderer_api/Texture.h"
#include "physics/RigidBody.h"
#include "Crate.h"

using namespace engine;

class ExampleScene : public Scene {

private:
	DebugCameraController* cameraInput;

	Texture* boxTexture;
	Mesh* boxMesh;

public:

	~ExampleScene();

	void onStart() override;

	void onUpdate(float deltaTime) override;
};