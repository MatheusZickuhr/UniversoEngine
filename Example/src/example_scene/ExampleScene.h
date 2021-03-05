#pragma once
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "utils/DebugCameraController.h"
#include "renderer/Mesh.h"
#include "renderer/renderer_api/Texture.h"
#include "physics/RigidBody.h"
#include "physics/CollisionMesh.h"
#include "Crate.h"

class ExampleScene : public engine::Scene {

private:
	std::unique_ptr<engine::DebugCameraController> cameraInput;

	std::shared_ptr<Crate> crate;
	std::shared_ptr<Crate> crateClone;

public:

	void onStart() override;

	void onUpdate(float deltaTime) override;
};