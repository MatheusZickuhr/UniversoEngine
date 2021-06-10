#include "DefaultLevelEditorScene.h"

DefaultLevelEditorScene::~DefaultLevelEditorScene() {

	delete cameraController;
}

void DefaultLevelEditorScene::onStart() {

	this->camera.position = { 0.0f, 0.0f, 0.0f };

	cameraController = new LevelEditorCameraController(this->camera);

	this->setSkyBoxCubeMap(&this->skyboxCubeMap);

	{
		auto box = createEntity();
		box->addComponent<NameComponent>("Box 1");
		box->addComponent<MeshComponent>(&boxMesh);
		box->addComponent<MaterialComponent>(&boxMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 8.0f, 0.0f, -4.0f };
	}

	{
		auto box = createEntity();
		box->addComponent<NameComponent>("Box 2");
		box->addComponent<MeshComponent>(&boxMesh);
		box->addComponent<MaterialComponent>(&boxMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { -8.0f, 0.0f, 4.0f };
	}

	{
		auto box = createEntity();
		box->addComponent<NameComponent>("Box 3");
		box->addComponent<MeshComponent>(&boxMesh);
		box->addComponent<MaterialComponent>(&boxMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 0.0f, 0.0f, -3.0f };
	}


	// create a floor entity
	{
		auto floor = createEntity();
		floor->addComponent<NameComponent>("Floor");
		floor->addComponent<MeshComponent>(&boxMesh);
		floor->addComponent<MaterialComponent>(&boxMaterial);
		floor->addComponent<TransformComponent>();

		auto& floorTransformComponent = floor->getComponent<TransformComponent>();
		floorTransformComponent.transform.position.y = -2.0f;
		floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

		floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
		floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);
	}

	// sun light
	{
		auto directionalLight = createEntity();
		directionalLight->addComponent<DirectionalLightComponent>();
		directionalLight->addComponent<NameComponent>("Directional Light");
		directionalLight->addComponent<TransformComponent>();
		auto& transComp = directionalLight->getComponent<TransformComponent>();
		transComp.transform.position = { -1.0f, 4.0f, -1.0f };
	}

	//{
	//	auto pointLight = createEntity();
	//	pointLight->addComponent<NameComponent>("Point Light");
	//	pointLight->addComponent<PointLightComponent>();
	//	pointLight->addComponent<TransformComponent>();
	//	auto& transComp = pointLight->getComponent<TransformComponent>();
	//	transComp.transform.position = { -1.0f, 2.0f, -1.0f };
	//}

}

void DefaultLevelEditorScene::onUpdate(float deltaTime) {
	cameraController->update(deltaTime);
}