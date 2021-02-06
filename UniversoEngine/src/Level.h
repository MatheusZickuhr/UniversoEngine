#pragma once

namespace engine {

	class LevelLoadingManager;

	class Level {

	private:

		std::vector<std::shared_ptr<GameObject>> gameObjects;

	protected:
		LevelLoadingManager* levelLoadingManager;
		std::shared_ptr<Camera> camera;

	public:

		Level() {
			this->camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		}

		void start() {
			this->onStart();
			this->startGameObjects();
		}

		void update(float deltaTime) {
			this->onUpdate(deltaTime);
			this->updateGameObjects(deltaTime);
		}

		void appendGameObject(std::shared_ptr<GameObject> gameObj) {
			this->gameObjects.push_back(gameObj);
		}

		void setLevelLoadingManager(LevelLoadingManager* levelManager) {
			this->levelLoadingManager = levelManager;
		} 

		const std::vector<std::shared_ptr<GameObject>>& getGameObjects() {
			return this->gameObjects;
		}

		std::shared_ptr<Camera> getCamera() {
			return this->camera;
		}

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

	private:

		void startGameObjects() {
			for (auto gameObject : this->gameObjects)
				gameObject->onStart();
		}

		void updateGameObjects(float deltaTime) {
			for (auto gameObject : this->gameObjects)
				gameObject->onUpdate(deltaTime);
		}

	};
}
