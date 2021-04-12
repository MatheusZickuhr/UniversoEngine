#include "FpsScene.h"

FpsScene::~FpsScene() {
	delete boxTexture;
	delete boxMesh;
	delete grassCubeTexture;
	delete grassCubeMesh;
}

void FpsScene::onStart() {
	boxTexture = new Texture("res/textures/crate/crate.jpg");
	boxMesh = new Mesh("res/models/crate/crate.obj");

	grassCubeTexture = new Texture("res/textures/grass_cube_texture/grass_cube_texture.png");
	grassCubeMesh = new Mesh("res/models/grass_cube/grass_cube.obj");

	auto player = createEntity();
	player->addComponent<TextureComponent>(boxTexture);
	player->addComponent<MeshComponent>(boxMesh);
	player->addComponent<TransformComponent>();
	player->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	player->addComponent<RigidBodyComponent>();
	player->addComponent<BehaviorComponent>()
		.bindBehavior<PlayerBehavior>();

	auto floor = createEntity();
	floor->addComponent<TextureComponent>(boxTexture);
	floor->addComponent<MeshComponent>(boxMesh);
	floor->addComponent<TransformComponent>();
	floor->getComponent<TransformComponent>().transform.position.y = -5.0f;
	floor->getComponent<TransformComponent>().transform.scale = { 10.0f, 0.5f, 10.0f };
	floor->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	floor->addComponent<RigidBodyComponent>(RigidBodyType::Static);


	auto obstacle1 = createEntity();
	obstacle1->addComponent<TextureComponent>(grassCubeTexture);
	obstacle1->addComponent<MeshComponent>(grassCubeMesh);
	obstacle1->addComponent<TransformComponent>();
	obstacle1->getComponent<TransformComponent>().transform.position.x = 5;
	obstacle1->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	obstacle1->addComponent<RigidBodyComponent>();
	

	auto obstacle2 = createEntity();
	obstacle2->addComponent<TextureComponent>(boxTexture);
	obstacle2->addComponent<MeshComponent>(boxMesh);
	obstacle2->addComponent<TransformComponent>();
	obstacle2->getComponent<TransformComponent>().transform.position.x = -5;
	obstacle2->addComponent<CollisionShapeComponent>(CollisionShape::Box);
	obstacle2->addComponent<RigidBodyComponent>();
	
}

void FpsScene::onUpdate(float deltaTime) {

}