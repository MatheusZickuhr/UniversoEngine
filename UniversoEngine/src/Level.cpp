#include "Level.h"

namespace engine {

	Level::Level() {
		this->camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	void Level::start() {
		this->onStart();
		this->startGameObjects();
	}

	void Level::update(float deltaTime) {
		this->onUpdate(deltaTime);
		this->updateGameObjects(deltaTime);
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

	void Level::startGameObjects() {
		for (auto gameObject : this->gameObjects)
			gameObject->onStart();
	}

	void Level::updateGameObjects(float deltaTime) {
		for (auto gameObject : this->gameObjects)
			gameObject->onUpdate(deltaTime);
	}


}