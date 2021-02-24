#include "Level.h"

namespace engine {

	Level::Level() {
		this->camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	}
	
	void Level::appendGameObject(std::shared_ptr<GameObject> gameObj) {
		this->gameObjects.push_back(gameObj);
	}

	const std::vector<std::shared_ptr<GameObject>>& Level::getGameObjects() {
		return this->gameObjects;
	}

	std::shared_ptr<Camera> Level::getCamera() {
		return this->camera;
	}

}