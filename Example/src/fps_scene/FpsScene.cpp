#include "FpsScene.h"

FpsScene::~FpsScene() {
	delete boxMaterial;
	delete boxMesh;
	delete grassCubeMaterial;
	delete grassCubeMesh;
}

void FpsScene::onStart() {
	boxMaterial = new Material("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");

	grassCubeMaterial = new Material("res/textures/grass_cube_texture/grass_cube_texture.png");
	grassCubeMesh = new Mesh("res/models/grass_cube/grass_cube.obj");

	// create a player entity
	auto player = createEntity();
	player->addComponent<MaterialComponent>(boxMaterial);
	player->addComponent<DynamicMeshComponent>(boxMesh);
	player->addComponent<TransformComponent>();
	player->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	player->addComponent<RigidBodyComponent>();
	player->addComponent<BehaviorComponent>()
		.bindBehavior<PlayerBehavior>();

	// create a floor entity
	auto floor = createEntity();
	floor->addComponent<MaterialComponent>(boxMaterial);
	floor->addComponent<DynamicMeshComponent>(boxMesh);
	floor->addComponent<TransformComponent>();

	auto& floorTransformComponent = floor->getComponent<TransformComponent>();
	floorTransformComponent.transform.position.y = -5.0f;
	floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

	floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);

	// create a obstacle entity
	auto obstacle1 = createEntity();
	obstacle1->addComponent<MaterialComponent>(grassCubeMaterial);
	obstacle1->addComponent<DynamicMeshComponent>(grassCubeMesh);
	obstacle1->addComponent<TransformComponent>();
	
	auto& obstacle1TransformComponent = obstacle1->getComponent<TransformComponent>();
	obstacle1TransformComponent.transform.position.x = 5;
	
	obstacle1->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	obstacle1->addComponent<RigidBodyComponent>();
	
	// create other obstacle entity
	auto obstacle2 = createEntity();
	obstacle2->addComponent<MaterialComponent>(boxMaterial);
	obstacle2->addComponent<DynamicMeshComponent>(boxMesh);
	obstacle2->addComponent<TransformComponent>();

	auto& obstacle2TransformComponent = obstacle2->getComponent<TransformComponent>();
	obstacle2TransformComponent.transform.position.x = -5;
	
	obstacle2->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	obstacle2->addComponent<RigidBodyComponent>();
	
}

void FpsScene::onUpdate(float deltaTime) {

}