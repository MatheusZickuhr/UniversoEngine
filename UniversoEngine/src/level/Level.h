#pragma once

#include <vector>
#include <memory>
#include "../game_object/GameObject.h"
#include "../renderer/Camera.h"

namespace engine {

	class LevelLoadingManager;

	class Level {

	private:

		std::vector<std::shared_ptr<GameObject>> gameObjects;

	protected:
		std::shared_ptr<Camera> camera;

	public:

		Level();

		void appendGameObject(std::shared_ptr<GameObject> gameObj);

		const std::vector<std::shared_ptr<GameObject>>& getGameObjects();

		std::shared_ptr<Camera> getCamera();

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

	};
}
