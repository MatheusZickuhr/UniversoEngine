#include "FpsScene.h"

FpsScene::~FpsScene() {
	delete texture;
	delete mesh;
}

void FpsScene::onStart() {
	texture = new engine::Texture("res/textures/crate/crate.jpg");
	mesh = new engine::Mesh("res/models/crate/crate.obj");

	auto player = createEntity();
	player->addComponent<engine::TextureComponent>(texture);
	player->addComponent<engine::MeshComponent>(mesh);
	player->addComponent<engine::TransformComponent>();
	player->addComponent<engine::RigidBodyComponent>();
	player->addComponent<engine::BehaviorComponent>()
		.bindBehavior<PlayerBehavior>();

	auto floor = createEntity();
	floor->addComponent<engine::TextureComponent>(texture);
	floor->addComponent<engine::MeshComponent>(mesh);
	floor->addComponent<engine::TransformComponent>();
	floor->addComponent<engine::RigidBodyComponent>();
	floor->getComponent<engine::RigidBodyComponent>().rigidBody->isStatic = true;
	floor->getComponent<engine::TransformComponent>().transform.position.y = -5.0f;
	floor->getComponent<engine::TransformComponent>().transform.scale = { 10.0f, 0.5f, 10.0f };

	auto obstacle1 = createEntity();
	obstacle1->addComponent<engine::TextureComponent>(texture);
	obstacle1->addComponent<engine::MeshComponent>(mesh);
	obstacle1->addComponent<engine::TransformComponent>();
	obstacle1->addComponent<engine::RigidBodyComponent>();
	obstacle1->getComponent<engine::TransformComponent>().transform.position.x = 5;

	auto obstacle2 = createEntity();
	obstacle2->addComponent<engine::TextureComponent>(texture);
	obstacle2->addComponent<engine::MeshComponent>(mesh);
	obstacle2->addComponent<engine::TransformComponent>();
	obstacle2->addComponent<engine::RigidBodyComponent>();
	obstacle2->getComponent<engine::TransformComponent>().transform.position.x = -5;
}

void FpsScene::onUpdate(float deltaTime) {

}