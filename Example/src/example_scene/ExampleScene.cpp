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
	boxMaterial = new Material("Example/resources/textures/crate/crate.jpg");
	boxMesh = new Mesh("Example/resources/models/crate/crate.obj");
	
	redMaterial = new Material();
	redMaterial->ambient = { 1.0f, 0.0f, 0.0f };
	redMaterial->diffuse = { 1.0f, 0.0f, 0.0f };

	Random random;

	const float PI = 3.14f;
	
	{
		auto box = createEntity();
		box->addComponent<MeshComponent>(boxMesh);
		box->addComponent<MaterialComponent>(redMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 3.0f, 0.0f, 0.0f };
	}

	{
		auto box = createEntity();
		box->addComponent<MeshComponent>(boxMesh);
		box->addComponent<MaterialComponent>(redMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { -3.0f, 0.0f, 0.0f };
	}

	{
		auto box = createEntity();
		box->addComponent<MeshComponent>(boxMesh);
		box->addComponent<MaterialComponent>(redMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 0.0f, 0.0f, -3.0f };
	}


	// create a floor entity
	{
		auto floor = createEntity();
		floor->addComponent<MeshComponent>(boxMesh);
		floor->addComponent<MaterialComponent>(boxMaterial);
		floor->addComponent<TransformComponent>();

		auto& floorTransformComponent = floor->getComponent<TransformComponent>();
		floorTransformComponent.transform.position.y = -2.0f;
		floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

		floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
		floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);
	}


	// sun light

	//{
	//	auto directionalLight = createEntity();
	//	directionalLight->addComponent<DirectionalLightComponent>();
	//	directionalLight->addComponent<TransformComponent>();
	//	auto& transComp = directionalLight->getComponent<TransformComponent>();
	//	transComp.transform.position = { -1.0f, 4.0f, -1.0f };
	//}

	//{
	//	auto directionalLight = createEntity();
	//	directionalLight->addComponent<DirectionalLightComponent>();
	//	directionalLight->addComponent<TransformComponent>();
	//	auto& transComp = directionalLight->getComponent<TransformComponent>();
	//	transComp.transform.position = { 1.0f, 4.0f, 1.0f };
	//}

	//{
	//	auto pointLight = createEntity();
	//	pointLight->addComponent<TransformComponent>();
	//	auto& transComp = pointLight->getComponent<TransformComponent>();
	//	transComp.transform.position = { 0.0f, 1.0f, 0.0f };
	//	pointLight->addComponent<PointLightComponent>();
	//}

	//{
	//	auto pointLight = createEntity();
	//	pointLight->addComponent<TransformComponent>();
	//	auto& transComp = pointLight->getComponent<TransformComponent>();
	//	transComp.transform.position = { -6.0f, 1.0f, 2.0f };
	//	pointLight->addComponent<PointLightComponent>();
	//}

	{
		auto pointLight = createEntity();
		pointLight->addComponent<TransformComponent>();
		auto& transComp = pointLight->getComponent<TransformComponent>();
		transComp.transform.position = { 6.0f, 1.0f, -2.0f };
		pointLight->addComponent<PointLightComponent>();
	}

}

void ExampleScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}