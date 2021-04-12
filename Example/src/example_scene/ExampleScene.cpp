#include "ExampleScene.h"

ExampleScene::~ExampleScene() {
	delete this->boxTexture;
	delete this->boxMesh;
	delete cameraInput;
}

void ExampleScene::onStart() {

	this->camera.position = { 0.0f, -3.0f, 20.0f };

	cameraInput = new CameraController(this->camera);
	boxTexture = new Texture("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");
	
	Random random;

	const float PI = 3.14f;

	// create some boxes
	for (float x = -6.0f; x <= 6.0f; x+= 2.0f) {
		for (float z = -6.0f; z <= 6.0f; z += 2.0f) {
			auto box = createEntity();
			box->addComponent<MeshComponent>(boxMesh);
			box->addComponent<TextureComponent>(boxTexture);
			box->addComponent<TransformComponent>();
			
			auto& boxTransformComponent = box->getComponent<TransformComponent>();
			boxTransformComponent.transform.scale *= 0.5f;
			boxTransformComponent.transform.position = { x, random.random() * 5.0f, z };
			boxTransformComponent.transform.rotation = { random.random() * PI, 0.0f, 0.0f };

			box->addComponent<CollisionShapeComponent>(CollisionShape::Box);
			box->addComponent<RigidBodyComponent>();
		}
	}

	// create a floor entity
	auto floor = createEntity();
	floor->addComponent<MeshComponent>(boxMesh);
	floor->addComponent<TextureComponent>(boxTexture);
	floor->addComponent<TransformComponent>();
	
	auto& floorTransformComponent = floor->getComponent<TransformComponent>();
	floorTransformComponent.transform.position.y = -5.0f;
	floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

	floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);

}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}