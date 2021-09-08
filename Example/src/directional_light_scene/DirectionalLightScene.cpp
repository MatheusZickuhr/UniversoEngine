#include "DirectionalLightScene.h"

DirectionalLightScene::~DirectionalLightScene() {

	delete cameraInput;
}

void DirectionalLightScene::onStart() {

	this->camera.position = { 0.0f, 0.0f, 5.0f };

	cameraInput = new CameraController(this->camera);

	this->setSkyBoxCubeMap(&this->skyboxCubeMap);

	{
		auto box = createEntity();
		box->addComponent<MaterialComponent>(&boxMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 4.0f, 0.0f, 0.0f };

		box->addComponent<StaticMeshComponent>(&boxMesh);
	}

	{
		auto box = createEntity();
		box->addComponent<MaterialComponent>(&boxMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { -4.0f, 0.0f, 0.0f  };

		box->addComponent<StaticMeshComponent>(&boxMesh);
	}

	{
		auto box = createEntity();
		box->addComponent<MaterialComponent>(&boxMaterial);
		box->addComponent<TransformComponent>();

		auto& boxTransformComponent = box->getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 0.0f, 0.0f, -4.0f };

		box->addComponent<StaticMeshComponent>(&boxMesh);
	}

	// create a floor entity
	{
		auto floor = createEntity();
		floor->addComponent<MaterialComponent>(&boxMaterial);
		floor->addComponent<TransformComponent>();

		auto& floorTransformComponent = floor->getComponent<TransformComponent>();
		floorTransformComponent.transform.position.y = -2.0f;
		floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

		floor->addComponent<StaticMeshComponent>(&boxMesh);

		floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
		floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);
	}

	// sun light
	{
		auto directionalLight = createEntity();
		directionalLight->addComponent<DirectionalLightComponent>();
		directionalLight->addComponent<TransformComponent>();
		auto& transComp = directionalLight->getComponent<TransformComponent>();
		transComp.transform.position = { -1.0f, 4.0f, -1.0f };
	}

}

void DirectionalLightScene::onUpdate(float deltaTime) {
	cameraInput->update(deltaTime);
}