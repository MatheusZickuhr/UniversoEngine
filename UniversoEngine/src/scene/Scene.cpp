#include "Scene.h"

#include "Components.h"
#include "Entity.h"

namespace engine {

	Scene::Scene() {
		this->camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	Scene::~Scene() {
		for (Entity* entity : entities) {
			delete entity;
		}
		entities.clear();
	}

	void Scene::start() {

		this->onStart();

		auto view = this->registry.view<BehaviorComponent>();
		for (auto [entity, behaviorComp] : view.each()) {
			behaviorComp.behavior->initialize();
			behaviorComp.behavior->onStart();
		}
	}

	void Scene::update(float deltaTime) {
		this->onUpdate(deltaTime);

		auto view = this->registry.view<BehaviorComponent>();

		for (auto [entity, behaviorComp] : view.each()) {

			if (!behaviorComp.behavior->isInitialized()) {
				behaviorComp.behavior->initialize();
				behaviorComp.behavior->onStart();
			}

			behaviorComp.behavior->onUpdate(deltaTime);
		}
	}

	std::shared_ptr<Camera> Scene::getCamera() {
		return this->camera;
	}

	entt::registry& Scene::getRegistry() {
		return this->registry;
	} 

	Entity* Scene::createEntity() {
		entt::entity enttEntity = this->registry.create();
		Entity* entity = new Entity{ enttEntity, this };
		this->entities.push_back(entity);
		return entity;
	}

}