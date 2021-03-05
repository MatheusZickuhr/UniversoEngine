#include "ExampleScene.h"

	void ExampleScene::onStart() {
		this->cameraInput = std::make_unique<engine::DebugCameraController>(this->camera);

		auto mesh = std::make_shared<engine::Mesh>("res/models/crate/crate.obj");
		auto texture = std::make_shared<engine::Texture>("res/textures/crate/crate.jpg");
		
		// create first entity

		auto transform  = std::make_shared<engine::Transform>();

		// rigid body creation (with collision mesh)
		std::vector<glm::vec3> vertices;
		for (auto vertex : mesh->vertices) vertices.push_back(vertex.position);
		
		auto collisionMesh = std::make_shared<engine::CollisionMesh>(vertices);
		auto rigidBody = std::make_shared<engine::RigidBody>(transform, collisionMesh);

		auto entity = createEntity();
		entity.addComponent<engine::MeshComponent>(mesh);
		entity.addComponent<engine::TextureComponent>(texture);
		entity.addComponent<engine::TransformComponent>(transform);
		entity.addComponent<engine::RigidBodyComponent>(rigidBody);
		
		auto t = entity.getComponent<engine::TransformComponent>();
		auto has = entity.hasComponent<engine::TransformComponent>();

		// create second entity

		auto transform2  = std::make_shared<engine::Transform>();
		transform2->position = {0.0f, -5.0f, 0.0f};

		// rigid body creation (with collision mesh)
		std::vector<glm::vec3> vertices2;
		for (auto vertex : mesh->vertices) vertices2.push_back(vertex.position);
		
		auto collisionMesh2 = std::make_shared<engine::CollisionMesh>(vertices2);
		auto rigidBody2 = std::make_shared<engine::RigidBody>(transform2, collisionMesh2);
		rigidBody2->isStatic = true;

		auto entity2 = registry.create();
		registry.emplace<engine::MeshComponent>(entity2, mesh);
		registry.emplace<engine::TextureComponent>(entity2, texture);
		registry.emplace<engine::TransformComponent>(entity2, transform2);
		registry.emplace<engine::RigidBodyComponent>(entity2, rigidBody2);

	}

    void ExampleScene::onUpdate(float deltaTime) {
		cameraInput->update(deltaTime);
	}