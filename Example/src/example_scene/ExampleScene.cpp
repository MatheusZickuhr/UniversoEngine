#include "ExampleScene.h"

ExampleScene::~ExampleScene() {
	delete this->boxMaterial;
	delete this->boxMesh;
	delete redMaterial;
	delete cameraInput;
}

void ExampleScene::onStart() {

	this->camera.position = { 0.0f, -3.0f, 20.0f };

	cameraInput = new CameraController(this->camera);
	boxMaterial = new Material("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");
	
	redMaterial = new Material(glm::vec3(1.0f, 0.5f, 0.31f));

	Random random;

	const float PI = 3.14f;

	// create some boxes
	for (float x = -6.0f; x <= 6.0f; x+= 2.0f) {
		for (float z = -6.0f; z <= 6.0f; z += 2.0f) {
			auto box = createEntity();
			box->addComponent<MeshComponent>(boxMesh);
			box->addComponent<MaterialComponent>(redMaterial);
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
	floor->addComponent<MaterialComponent>(boxMaterial);
	floor->addComponent<TransformComponent>();
	
	auto& floorTransformComponent = floor->getComponent<TransformComponent>();
	floorTransformComponent.transform.position.y = -5.0f;
	floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

	floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);

	//auto pointLight = createEntity();
	//pointLight->addComponent<TransformComponent>();
	//pointLight->addComponent<PointLightComponent>();

	// sun light
	auto directionalLight = createEntity();
	directionalLight->addComponent<DirectionalLightComponent>();
	directionalLight->getComponent<DirectionalLightComponent>().directionalLight.direction = { -0.2f, -1.0f, -0.3f };
}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}