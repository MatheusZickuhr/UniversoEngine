#include "DirectionalLightScene.h"


DirectionalLightScene::~DirectionalLightScene() {

}

void DirectionalLightScene::onStart() {
	this->camera3d->position = { -2.0f, 3.0f, 10.0f };
	cameraController = std::make_unique<Camera3dController>(this->window, this->camera3d);

	boxMaterial = std::make_shared<Material>("Example/resources/textures/crate/crate.jpg");
	boxMesh = std::make_shared<Mesh>("Example/resources/models/crate/crate.obj");
	
	lowPolyTree1Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree1/lowPolyTree1.png");
	lowPolyTree1Mesh = std::make_shared<Mesh >("Example/resources/models/lowPolyTree1/lowPolyTree1.obj");
	
	lowPolyTree2Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree2/lowPolyTree2.png");
	lowPolyTree2Mesh = std::make_shared<Mesh>("Example/resources/models/lowPolyTree2/lowPolyTree2.obj");
	
	lowPolyTree3Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree3/lowPolyTree3.png");
	lowPolyTree3Mesh = std::make_shared<Mesh>("Example/resources/models/lowPolyTree3/lowPolyTree3.obj");
	
	lowPolyTree4Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree4/lowPolyTree4.png");
	lowPolyTree4Mesh = std::make_shared<Mesh>("Example/resources/models/lowPolyTree4/lowPolyTree4.obj");
	
	terrainMaterial = std::make_shared<Material>();

	cubeMesh = std::make_shared<Mesh>("Example/resources/models/cube/cube.obj");

	this->skyboxCubeMap = CubeMap::createCubeMapFromFile(
		{
			"Example/resources/textures/exampleSkyBox/right.jpg",
			"Example/resources/textures/exampleSkyBox/left.jpg",
			"Example/resources/textures/exampleSkyBox/top.jpg",
			"Example/resources/textures/exampleSkyBox/bottom.jpg",
			"Example/resources/textures/exampleSkyBox/front.jpg",
			"Example/resources/textures/exampleSkyBox/back.jpg"
		}
	);

	this->setSkyBoxCubeMap(this->skyboxCubeMap);


	{
		auto box = createEntity();
		box.addComponent<MaterialComponent>(boxMaterial);
		box.addComponent<TransformComponent>();

		auto& boxTransformComponent = box.getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 4.0f, 60.0f, 0.0f };

		box.addComponent<DynamicMeshComponent>(boxMesh);

		box.addComponent<CollisionShapeComponent>(CollisionShape::Box);
		box.addComponent<RigidBodyComponent>(RigidBodyType::Dynamic);
	}

	{
		auto lowPolyTree1 = createEntity();
		lowPolyTree1.addComponent<MaterialComponent>(lowPolyTree1Material);
		lowPolyTree1.addComponent<TransformComponent>();

		auto& transformComponent = lowPolyTree1.getComponent<TransformComponent>();
		transformComponent.transform.position = { -5.0f, 0.0f, 3.0f };

		lowPolyTree1.addComponent<DynamicMeshComponent>(lowPolyTree1Mesh);
	}

	{
		auto lowPolyTree2 = createEntity();
		lowPolyTree2.addComponent<MaterialComponent>(lowPolyTree2Material);
		lowPolyTree2.addComponent<TransformComponent>();

		auto& transformComponent = lowPolyTree2.getComponent<TransformComponent>();
		transformComponent.transform.position = { -1.0f, 0.0f, 3.0f };

		lowPolyTree2.addComponent<DynamicMeshComponent>(lowPolyTree2Mesh);
	}

	{
		auto lowPolyTree3 = createEntity();
		lowPolyTree3.addComponent<MaterialComponent>(lowPolyTree3Material);
		lowPolyTree3.addComponent<TransformComponent>();

		auto& transformComponent = lowPolyTree3.getComponent<TransformComponent>();
		transformComponent.transform.position = { -1.0f, 0.0f, 0.0f };

		lowPolyTree3.addComponent<StaticMeshComponent>(lowPolyTree3Mesh);
	}

	{
		auto lowPolyTree4 = createEntity();
		lowPolyTree4.addComponent<MaterialComponent>(lowPolyTree4Material);
		lowPolyTree4.addComponent<TransformComponent>();

		auto& transformComponent = lowPolyTree4.getComponent<TransformComponent>();
		transformComponent.transform.position = { -5.0f, 0.0f, 0.0f };

		lowPolyTree4.addComponent<StaticMeshComponent>(lowPolyTree4Mesh);
	}

	{

		terrainMaterial->ambient = { 0.28f, 0.43f, 0.21f };
		terrainMaterial->diffuse = { 0.28f, 0.43f, 0.21f };

		auto terrain = createEntity();
		terrain.addComponent<MaterialComponent>(terrainMaterial);
		terrain.addComponent<TransformComponent>();

		auto& transformComponent = terrain.getComponent<TransformComponent>();
		transformComponent.transform.scale = { 10.0f, 0.1f, 10.0f };

		terrain.addComponent<StaticMeshComponent>(cubeMesh);

		terrain.addComponent<CollisionShapeComponent>(CollisionShape::Box);
		terrain.addComponent<RigidBodyComponent>(RigidBodyType::Static);
	}

	// sun light
	{
		auto directionalLight = createEntity();
		directionalLight.addComponent<DirectionalLightComponent>();
		directionalLight.addComponent<TransformComponent>();
		auto& transComp = directionalLight.getComponent<TransformComponent>();
		transComp.transform.position = { -1.0f, 4.0f, -1.0f };
	}

}

void DirectionalLightScene::onUpdate(float deltaTime) {
	cameraController->update(deltaTime);
}