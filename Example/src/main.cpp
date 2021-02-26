
#include "Engine.h"
#include "utils/DebugCameraController.h"

class Crate : public engine::GameObject {

public:

	using engine::GameObject::GameObject;

	void onStart() override {

	}

	void onUpdate(float deltaTime) override {

	}
};

class ExampleLevel : public engine::Level {

private:
	std::unique_ptr<engine::DebugCameraController> cameraInput;

	std::shared_ptr<Crate> crate;
	std::shared_ptr<Crate> crateClone;

public:

	void onStart() override {
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

	void onUpdate(float deltaTime) override {
		cameraInput->update(deltaTime);

	}
};

int main() {
	ExampleLevel* initialLevel = new ExampleLevel();
	engine::Engine* engine = new engine::Engine(initialLevel, 800, 600, "Example");

	while (engine->isRunning()) {
		engine->tick();
	}

	delete initialLevel;
	delete engine;

	return 0;
}
