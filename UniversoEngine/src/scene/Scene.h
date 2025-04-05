#pragma once

#include <vector>
#include <memory>
#include <optional>

#include <entt/entt.hpp>

#include "../renderer/Camera.h"
#include "../renderer/Renderer3D.h"
#include "../renderer/Renderer2D.h"
#include "../renderer/renderer_api/CubeMap.h"
#include "../physics/PhysicsWorld.h"
#include "../physics/RigidBody.h"

#include "NewSceneListener.h"

#include "../Window.h"


namespace engine {
	

	class Application;
	class Entity;

	class Scene {

	public:

		Scene();

		virtual ~Scene();

		void initialize(
			std::shared_ptr<Window> window,
			std::shared_ptr<PhysicsWorld> physicsWorld,
			std::shared_ptr<Renderer3D> renderer3d,
			std::shared_ptr<Renderer2D> renderer2d);

		void onUpdateCallBack(float deltaTime);

		void render();
		
		void updatePhysicsWorld(float deltaTime);
		
		void updatePhysics(float timeInterpolationFactor);

		Camera& getCamera();

		entt::registry& getRegistry();
		
		Entity createEntity();

		void destroyEntity(Entity& entity);

		bool isInitialized() { return initialized; }

		std::shared_ptr<Window> getWindow();

		void setNewSceneListener(NewSceneListener* newSceneListener);
	
	protected:

		Camera camera;

		std::shared_ptr<Window> window;

		void setSkyBoxCubeMap(std::shared_ptr<CubeMap> skyBoxCubeMap) { renderer3d->setSkyBoxCubeMap(skyBoxCubeMap); }

		void loadNewScene(std::unique_ptr<Scene> scene);

	private:

		bool initialized;

		std::shared_ptr<PhysicsWorld> physicsWorld;
		std::shared_ptr<Renderer3D> renderer3d;
		std::shared_ptr<Renderer2D> renderer2d;

		entt::registry registry;

		NewSceneListener* newSceneListener = nullptr;

		std::optional<Entity> findEntityByCollisionBody(std::shared_ptr<CollisionBody> collisionBody);

		void onStaticMeshComponentCreated(entt::registry& registry, entt::entity entity);

		void onRigidBodyComponentCreated(entt::registry& registry, entt::entity entity);

		void onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity);

		void onCollisionBodyComponentCreated(entt::registry& registry, entt::entity entity);

		void onCollisionBodyComponentDestroyed(entt::registry& registry, entt::entity entity);

		void onBehaviorComponentDestroyed(entt::registry& registry, entt::entity entity);

		void onStaticMeshComponentDestroyed(entt::registry& registry, entt::entity entity);

		std::string generateUuid();

		virtual void onStart() = 0;
		
		virtual void onUpdate(float deltaTime) = 0;

	};
}
