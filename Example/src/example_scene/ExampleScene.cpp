#include "ExampleScene.h"
#include "scene/Components.h"

ExampleScene::~ExampleScene() {
	delete this->boxTexture;
	delete this->boxMesh;
	delete cameraInput;
}

void ExampleScene::onStart() {
	cameraInput = new DebugCameraController(this->camera);
	boxTexture = new Texture("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");
	
	auto entity = createEntity();
	entity->addComponent<MeshComponent>(boxMesh);
	entity->addComponent<TextureComponent>(boxTexture);
	entity->addComponent<TransformComponent>();
	entity->addComponent<RigidBodyComponent>();
	entity->addComponent<BehaviorComponent>().bindBehavior<CrateBehavior>();
}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}