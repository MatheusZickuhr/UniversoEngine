#pragma once

#include <memory>
#include <iostream>
#include "Level.h"

namespace engine {

	class LevelLoadingManager {

	private: 
		std::shared_ptr<Level> currentLevel;

	public:

		LevelLoadingManager();

		template <typename T>
		void loadLevel() {
			this->currentLevel = std::make_shared<T>();
			this->currentLevel->setLevelLoadingManager(this);
			this->currentLevel->start();
		}

		std::shared_ptr<Level> getCurrentLevel();

	};
}