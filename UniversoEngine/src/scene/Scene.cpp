#include "Scene.h"
#include "Entity.h"

namespace engine {

	Scene::Scene() {
		this->camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	}
	
	void Scene::appendGameObject(std::shared_ptr<GameObject> gameObj) {
		this->gameObjects.push_back(gameObj);
	}

	const std::vector<std::shared_ptr<GameObject>>& Scene::getGameObjects() {
		return this->gameObjects;
	}

	std::shared_ptr<Camera> Scene::getCamera() {
		return this->camera;
	}

	Entity Scene::createEntity() {
		entt::entity entity = this->registry.create();
		return Entity {entity, this}; 
	}

}