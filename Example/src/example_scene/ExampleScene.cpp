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
	entity->getComponent<TransformComponent>().transform.position = {0.0f, 1.0f, 0.0f };
	entity->getComponent<TransformComponent>().transform.rotation = {0.1f * 3.14f, 0.0f, 0.0f };
	entity->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	entity->addComponent<RigidBodyComponent>();

	auto floor = createEntity();
	floor->addComponent<MeshComponent>(boxMesh);
	floor->addComponent<TextureComponent>(boxTexture);
	floor->addComponent<TransformComponent>();
	floor->getComponent<TransformComponent>().transform.position.y = -5.0f;
	floor->getComponent<TransformComponent>().transform.scale = { 10.0f, 0.5f, 10.0f };
	floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	floor->addComponent<RigidBodyComponent>(false);

}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}