#include "ExampleScene.h"

ExampleScene::~ExampleScene() {
	delete this->boxMaterial;
	delete this->boxMesh;
	delete redMaterial;
	delete cameraInput;
}

void ExampleScene::onStart() {

	this->camera.position = { 0.0f, 0.0f, 4.0f };

	cameraInput = new CameraController(this->camera);
	boxMaterial = new Material("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");
	
	redMaterial = new Material();
	redMaterial->ambient = { 1.0f, 0.0f, 0.0f };
	redMaterial->diffuse = { 1.0f, 0.0f, 0.0f };

	Random random;

	const float PI = 3.14f;

	box = createEntity();
	box->addComponent<MeshComponent>(boxMesh);
	box->addComponent<MaterialComponent>(redMaterial);
	box->addComponent<TransformComponent>();
			
	auto& boxTransformComponent = box->getComponent<TransformComponent>();
	boxTransformComponent.transform.position = { 0.0f, 0.0f, 0.0f };

	// create a floor entity
	auto floor = createEntity();
	floor->addComponent<MeshComponent>(boxMesh);
	floor->addComponent<MaterialComponent>(boxMaterial);
	floor->addComponent<TransformComponent>();
	
	auto& floorTransformComponent = floor->getComponent<TransformComponent>();
	floorTransformComponent.transform.position.y = -2.0f;
	floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

	floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);

	// sun light

	{
		auto directionalLight = createEntity();
		directionalLight->addComponent<DirectionalLightComponent>();
		directionalLight->getComponent<DirectionalLightComponent>().directionalLight.position = { -2.0f, 4.0f, -1.0f };
	}

	//{
	//	auto directionalLight = createEntity();
	//	directionalLight->addComponent<DirectionalLightComponent>();
	//	directionalLight->getComponent<DirectionalLightComponent>().directionalLight.position = { 2.0f, 4.0f, 1.0f };
	//}

}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);

	auto& boxTransformComponent = box->getComponent<TransformComponent>();
	boxTransformComponent.transform.rotation.x += 0.1f * deltaTime;
}