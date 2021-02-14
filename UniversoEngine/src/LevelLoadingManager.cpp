#include "LevelLoadingManager.h"

namespace engine {

	LevelLoadingManager::LevelLoadingManager() {}

	std::shared_ptr<Level> LevelLoadingManager::getCurrentLevel() {
		if (this->currentLevel == nullptr) {

			std::cout << "The engine requires a inicial level" << std::endl;
#ifdef _DEBUG
			__debugbreak();
#endif
		}
		return this->currentLevel;
	}


}