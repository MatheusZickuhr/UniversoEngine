#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "../renderer/Camera.h"
#include "../renderer/Renderer3D.h"
#include "../renderer/Renderer2D.h"
#include "../renderer/renderer_api/Texture2D.h"
#include "../physics/ReactPhysics3dPhysicsWorld.h"
#include "../physics/RigidBody.h"

namespace engine {

	class Entity;

	class Scene {

	public:

		Scene();

		~Scene();

		void onStartCallBack();

		void onUpdateCallBack(float deltaTime);

		void render();
		
		void updatePhysicsWorld(float deltaTime);
		
		void updatePhysics(float timeInterpolationFactor);

		Camera& getCamera();

		entt::registry& getRegistry();

	protected:

		Camera camera;

		Entity* createEntity();

		void setSkyBoxCubeMap(CubeMap* skyBoxCubeMap) { this->renderer3d.setSkyBoxCubeMap(skyBoxCubeMap); }

	private:

		PhysicsWorld* physicsWorld;
		Renderer3D renderer3d;
		Renderer2D renderer2d;
		entt::registry registry;
		std::vector<Entity*> entities;

		void onStaticMeshComponentCreated(entt::registry& registry, entt::entity entity);

		void onRigidBodyComponentCreated(entt::registry& registry, entt::entity entity);

		void onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity);

		void onBehaviorComponentDestroyed(entt::registry& registry, entt::entity entity);

		virtual void onStart() = 0;
		
		virtual void onUpdate(float deltaTime) = 0;

	};
}
