#pragma once

#include "Crate.h"
#include "DebugCameraController.h"

class ExampleLevel : public engine::Level {

private:
	std::unique_ptr<DebugCameraController> cameraInput;

public:

	void onStart() override {
		this->cameraInput = std::make_unique<DebugCameraController>(this->camera);

		auto mesh = std::make_shared<engine::Mesh>("res/models/crate/crate.obj");
		auto texture = std::make_shared<engine::Texture>("res/textures/crate/crate.jpg");

		auto crate = std::make_shared<Crate>(mesh, texture);

		auto crateClone = std::make_shared<Crate>(mesh, texture);
		crateClone->position = glm::vec3(3.0f, 0.0f, 0.0f);

		this->appendGameObject(crate);
		this->appendGameObject(crateClone);
	}

	void onUpdate(float deltaTime) override {
		cameraInput->update(deltaTime);
	}
};

