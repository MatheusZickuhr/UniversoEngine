#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"
#include "renderer/Camera.h"

namespace engine {

	class LevelLoadingManager;

	class Level {

	private:

		std::vector<std::shared_ptr<GameObject>> gameObjects;

	protected:
		LevelLoadingManager* levelLoadingManager;
		std::shared_ptr<Camera> camera;

	public:

		Level();

		void start();

		void update(float deltaTime);

		void appendGameObject(std::shared_ptr<GameObject> gameObj);

		void setLevelLoadingManager(LevelLoadingManager* levelManager);

		const std::vector<std::shared_ptr<GameObject>>& getGameObjects();

		std::shared_ptr<Camera> getCamera();

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

	private:

		void startGameObjects();

		void updateGameObjects(float deltaTime);

	};
}
