#pragma once

#include <vector>
#include <memory>

#include <entt/entt.hpp>

#include "../renderer/Camera.h"

namespace engine {

	class Entity;

	class Scene {
	protected:
		std::shared_ptr<Camera> camera;

	private:
		entt::registry registry;
		std::vector<Entity*> entities;
		
	public:

		Scene();

		~Scene();
		
		std::shared_ptr<Camera> getCamera();

		entt::registry& getRegistry();

		Entity* createEntity();

		void update(float deltaTime);

		void start();

	private:

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

	};
}
