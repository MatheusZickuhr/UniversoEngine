#pragma once

#include <vector>
#include <memory>
#include "../renderer/Camera.h"
#include "GameObject.h"
#include "entt/entt.hpp"

#include "MeshComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

namespace engine {

	class Scene {

	private:
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		
	protected:
		std::shared_ptr<Camera> camera;

	public:
		entt::registry registry;

		Scene();
		
		void appendGameObject(std::shared_ptr<GameObject> gameObj);

		const std::vector<std::shared_ptr<GameObject>>& getGameObjects();

		std::shared_ptr<Camera> getCamera();

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

	};
}
