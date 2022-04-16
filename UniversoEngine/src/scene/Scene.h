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
		
		Entity createEntity();

		void destroyEntity(Entity& entity);
	
	protected:

		Camera camera;

		void setSkyBoxCubeMap(std::shared_ptr<CubeMap> skyBoxCubeMap) { this->renderer3d.setSkyBoxCubeMap(skyBoxCubeMap); }

	private:

		PhysicsWorld* physicsWorld;
		Renderer3D renderer3d;
		Renderer2D renderer2d;
		entt::registry registry;

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
