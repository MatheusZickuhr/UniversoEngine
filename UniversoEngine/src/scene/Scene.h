#pragma once

#include <vector>
#include <memory>
#include "../renderer/Camera.h"
#include "GameObject.h"
#include "entt/entt.hpp"

namespace engine {

	class Entity;

	class Scene {

	private:
		std::vector<Entity*> entities;
		
	protected:
		std::shared_ptr<Camera> camera;

	public:
		entt::registry registry;

		Scene();

		~Scene();
		
		std::shared_ptr<Camera> getCamera();

		Entity* createEntity();

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

	};
}
