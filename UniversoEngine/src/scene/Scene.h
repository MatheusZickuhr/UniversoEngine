#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "../renderer/Camera.h"
#include "../renderer/Renderer3D.h"
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

		void render(float windowWidth, float windowHeight);

		void updatePhysicsWorld(float deltaTime);
		
		void updatePhysics(float timeInterpolationFactor);

		Camera& getCamera();

		entt::registry& getRegistry();
		
		Renderer3D* getRenderer();

	protected:

		Camera camera;

		Entity* createEntity();

	private:

		PhysicsWorld* physicsWorld;
		Renderer3D* renderer;
		entt::registry registry;
		std::vector<Entity*> entities;

		void onRigidBodyComponentCreated(entt::registry& registry, entt::entity entity);

		void onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity);

		void onBehaviorComponentDestroyed(entt::registry& registry, entt::entity entity);

		virtual void onStart() = 0;
		
		virtual void onUpdate(float deltaTime) = 0;

	};
}
