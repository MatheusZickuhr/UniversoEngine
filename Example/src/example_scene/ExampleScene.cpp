#include "ExampleScene.h"
#include "scene/Components.h"

ExampleScene::~ExampleScene() {
	delete this->boxTexture;
	delete this->boxMesh;
	delete cameraInput;
}

void ExampleScene::onStart() {

	this->camera.position = { 0.0f, -3.0f, 20.0f };

	cameraInput = new DebugCameraController(this->camera);
	boxTexture = new Texture("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");
	
	Random random;

	for (float x = -6.0f; x <= 6.0f; x+= 2.0f) {
		for (float z = -6.0f; z <= 6.0f; z += 2.0f) {
			auto entity = createEntity();
			entity->addComponent<MeshComponent>(boxMesh);
			entity->addComponent<TextureComponent>(boxTexture);
			entity->addComponent<TransformComponent>();
			entity->getComponent<TransformComponent>().transform.scale *= 0.5f;
			entity->getComponent<TransformComponent>().transform.position = { x, random.random() * 5.0f, z };
			entity->getComponent<TransformComponent>().transform.rotation = { random.random() * 3.14f, 0.0f, 0.0f };
			entity->addComponent<CollisionShapeComponent>(CollisionShape::Box);
			entity->addComponent<RigidBodyComponent>();
		}

	}



	auto floor = createEntity();
	floor->addComponent<MeshComponent>(boxMesh);
	floor->addComponent<TextureComponent>(boxTexture);
	floor->addComponent<TransformComponent>();
	floor->getComponent<TransformComponent>().transform.position.y = -5.0f;
	floor->getComponent<TransformComponent>().transform.scale = { 10.0f, 0.5f, 10.0f };
	floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);

}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}