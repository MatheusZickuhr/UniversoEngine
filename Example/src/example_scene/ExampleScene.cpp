#include "ExampleScene.h"

	void ExampleScene::onStart() {
		this->cameraInput = std::make_unique<engine::DebugCameraController>(this->camera);

		auto mesh = std::make_shared<engine::Mesh>("res/models/crate/crate.obj");
		auto texture = std::make_shared<engine::Texture>("res/textures/crate/crate.jpg");

		crate = std::make_shared<Crate>(mesh, texture);

		crateClone = std::make_shared<Crate>(mesh, texture);
		crateClone->transform->position = { 0.5f, -7.0f, 0.0f };
		crateClone->rigidBody->isStatic = true;


		this->appendGameObject(crate);
		this->appendGameObject(crateClone);
	}

    void ExampleScene::onUpdate(float deltaTime) {
		cameraInput->update(deltaTime);
	}