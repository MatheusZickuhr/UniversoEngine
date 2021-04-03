#include "FpsScene.h"

FpsScene::~FpsScene() {
	delete boxTexture;
	delete boxMesh;
	delete grassCubeTexture;
	delete grassCubeMesh;
}

void FpsScene::onStart() {
	boxTexture = new engine::Texture("res/textures/crate/crate.jpg");
	boxMesh = new engine::Mesh("res/models/crate/crate.obj");

	grassCubeTexture = new engine::Texture("res/textures/grass_cube_texture/grass_cube_texture.png");
	grassCubeMesh = new engine::Mesh("res/models/grass_cube/grass_cube.obj");

	auto player = createEntity();
	player->addComponent<engine::TextureComponent>(boxTexture);
	player->addComponent<engine::MeshComponent>(boxMesh);
	player->addComponent<engine::TransformComponent>();
	player->addComponent<engine::CollisionShapeComponent>(engine::CollisionShape::Box);
	player->addComponent<engine::RigidBodyComponent>();
	player->addComponent<engine::BehaviorComponent>()
		.bindBehavior<PlayerBehavior>();

	auto floor = createEntity();
	floor->addComponent<engine::TextureComponent>(boxTexture);
	floor->addComponent<engine::MeshComponent>(boxMesh);
	floor->addComponent<engine::TransformComponent>();
	floor->getComponent<engine::TransformComponent>().transform.position.y = -5.0f;
	floor->getComponent<engine::TransformComponent>().transform.scale = { 10.0f, 0.5f, 10.0f };
	floor->addComponent<engine::CollisionShapeComponent>(engine::CollisionShape::Box);
	floor->addComponent<engine::RigidBodyComponent>(false);


	auto obstacle1 = createEntity();
	obstacle1->addComponent<engine::TextureComponent>(grassCubeTexture);
	obstacle1->addComponent<engine::MeshComponent>(grassCubeMesh);
	obstacle1->addComponent<engine::TransformComponent>();
	obstacle1->getComponent<engine::TransformComponent>().transform.position.x = 5;
	obstacle1->addComponent<engine::CollisionShapeComponent>(engine::CollisionShape::Box);
	obstacle1->addComponent<engine::RigidBodyComponent>();
	

	auto obstacle2 = createEntity();
	obstacle2->addComponent<engine::TextureComponent>(boxTexture);
	obstacle2->addComponent<engine::MeshComponent>(boxMesh);
	obstacle2->addComponent<engine::TransformComponent>();
	obstacle2->getComponent<engine::TransformComponent>().transform.position.x = -5;
	obstacle2->addComponent<engine::CollisionShapeComponent>(engine::CollisionShape::Box);
	obstacle2->addComponent<engine::RigidBodyComponent>();
	
}

void FpsScene::onUpdate(float deltaTime) {

}