
#include "Engine.h"
#include "DebugCameraController.h"

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
		crate->transform->rotationAxis = { 1.0f, 0.0f, 0.0f };

		crateClone = std::make_shared<Crate>(mesh, texture);
		crateClone->transform->position = { 3.0f, 0.0f, 0.0f };
		crateClone->transform->rotationAxis = { 1.0f, 0.0f, 0.0f };

		this->appendGameObject(crate);
		this->appendGameObject(crateClone);
	}

	void onUpdate(float deltaTime) override {
		cameraInput->update(deltaTime);

		constexpr float speed = 25.0f;

		crate->transform->rotationAngle += deltaTime * speed;
		crateClone->transform->rotationAngle -= deltaTime * speed;
	}
};

int main() {
	engine::Engine engine(800, 600, "Universo Engine");
	engine.getLevelLoadingManager()->loadLevel<ExampleLevel>();
	engine.run();
	return 0;
}
