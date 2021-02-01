#pragma once

namespace engine {

	class LevelManager {
	private: 
		std::shared_ptr<Level> currentLevel;

	public:

		template <typename T>
		void loadLevel() {
			this->currentLevel = std::make_shared<T>();
			this->currentLevel->setLevelManager(this);
			this->currentLevel->start();
		}

		std::shared_ptr<Level> getCurrentLevel() {
			if (this->currentLevel == nullptr) {

				std::cout << "The engine requires a inicial level" << std::endl;
#ifdef _DEBUG
				__debugbreak();
#endif
			}
			return this->currentLevel;
		}

	};
}