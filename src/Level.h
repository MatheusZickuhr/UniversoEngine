#pragma once

namespace engine {

	class Engine;

	class Level {
	
	private: 

		std::vector<GameObject*> gameObjects;

	protected:

		Engine* engine;

	public:

		~Level() {
			for (GameObject* gameObject : this->gameObjects)
				delete gameObject;

			this->gameObjects.clear();
		}
		
		void setEngineInstance(Engine* engine) {
			this->engine = engine;
		}

		template<typename T> 
		T* createGameObject() {
			T* gameObj = new T();
			this->gameObjects.push_back(gameObj);
			return gameObj;
		}

		/* performs a shallow copy of the game object */
		template<typename T>
		T* cloneGameObject(T* other) {
			T* gameObj = new T(*other);
			this->gameObjects.push_back(gameObj);
			return gameObj;
		}

		const std::vector<GameObject*>& getGameObjects() {
			return this->gameObjects;
		}
		
		void start() {
			this->onStart();
			this->startGameObjects();
		} 

		void update(float deltaTime) {
			this->onUpdate(deltaTime);
			this->updateGameObjects(deltaTime);
		} 

		virtual void onStart() = 0; 
		virtual void onUpdate(float deltaTime) = 0;

	private: 

		void startGameObjects() {
			for (GameObject* gameObject : this->gameObjects)
				gameObject->onStart();
		}

		void updateGameObjects(float deltaTime) {
			for (GameObject* gameObject : this->gameObjects)
				gameObject->onUpdate(deltaTime);
		}

	};
}
