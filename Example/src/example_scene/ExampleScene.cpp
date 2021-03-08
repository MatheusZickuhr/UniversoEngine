#include "ExampleScene.h"

ExampleScene::~ExampleScene() {
	delete this->boxTexture;
	delete this->boxMesh;
	delete cameraInput;
}

void ExampleScene::onStart() {
	cameraInput = new DebugCameraController(this->camera);
	boxTexture = new Texture("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");
	
	// used for physics collision
	auto boxCollisionMesh  = boxMesh->getVeticesPositions();

	auto entity = createEntity();
	entity.addComponent<MeshComponent>(boxMesh);
	entity.addComponent<TextureComponent>(boxTexture);
	entity.addComponent<TransformComponent>();
	entity.addComponent<RigidBodyComponent>();

}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}