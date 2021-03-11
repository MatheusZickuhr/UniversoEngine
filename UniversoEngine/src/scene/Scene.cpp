#include "Scene.h"
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
	
	std::shared_ptr<Camera> Scene::getCamera() {
		return this->camera;
	}

	Entity* Scene::createEntity() {
		entt::entity enttEntity = this->registry.create();
		Entity* entity = new Entity {enttEntity, this};
		this->entities.push_back(entity);
		return entity;
	}

}