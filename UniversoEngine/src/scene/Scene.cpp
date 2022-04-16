#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "../debug/Assert.h"
#include <random>

namespace engine {

	Scene::Scene() :
		physicsWorld(new PhysicsWorld()) {
	
		this->registry.on_construct<StaticMeshComponent>()
			.connect<&Scene::onStaticMeshComponentCreated>(this);

		this->registry.on_construct<RigidBodyComponent>()
			.connect<&Scene::onRigidBodyComponentCreated>(this);

		this->registry.on_destroy<RigidBodyComponent>()
			.connect<&Scene::onRigidBodyComponentDestroyed>(this);

		this->registry.on_construct<CollisionBodyComponent>()
			.connect<&Scene::onCollisionBodyComponentCreated>(this);

		this->registry.on_destroy<CollisionBodyComponent>()
			.connect<&Scene::onCollisionBodyComponentDestroyed>(this);

		this->registry.on_destroy<BehaviorComponent>()
			.connect<&Scene::onBehaviorComponentDestroyed>(this);

		this->registry.on_destroy<StaticMeshComponent>()
			.connect<&Scene::onStaticMeshComponentDestroyed>(this);
	}

	Scene::~Scene() {
		delete this->physicsWorld;
	}

	void Scene::onStartCallBack() {

		this->onStart();

		auto view = this->registry.view<BehaviorComponent>();
		for (auto [entity, behaviorComp] : view.each()) {
			behaviorComp.behavior->initialize();
			behaviorComp.behavior->onStart();
		}
	}

	void Scene::render() {

		this->renderer3d.clearColor(0.0f, 0.0f, 0.0f, 1.0f);

		this->renderer3d.beginFrame(camera);
			// draw/ update lights
			{
				auto view = this->registry.view<PointLightComponent, TransformComponent>();

				for (auto [entity, lightComp, transComp] : view.each()) {
					lightComp.pointLight.position = transComp.transform.getTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					this->renderer3d.drawPointLight(lightComp.pointLight);
				}
			}

			{
				auto view = this->registry.view<DirectionalLightComponent, TransformComponent>();

				for (auto [entity, lightComp, transComp] : view.each()) {
					lightComp.directionalLight.position = transComp.transform.getTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					this->renderer3d.drawDirectionalLight(lightComp.directionalLight);
				}
			}

			// end update/draw lights
			{
				auto view = this->registry.view<DynamicMeshComponent, MaterialComponent, TransformComponent>();

				for (auto [entity, meshComp, materialComp, transComp] : view.each()) {
				
					this->renderer3d.drawDynamicMesh(
						meshComp.mesh,
						materialComp.material,
						transComp.transform.getTransformMatrix(),
						(uint32_t)entity);
				}
			}
		this->renderer3d.endFrame();
	}

	void Scene::onUpdateCallBack(float deltaTime) {
		this->onUpdate(deltaTime);

		auto view = this->registry.view<BehaviorComponent>();

		for (auto [entity, behaviorComp] : view.each()) {

			if (!behaviorComp.behavior->isInitialized()) {
				behaviorComp.behavior->initialize();
				behaviorComp.behavior->onStart();
			}

			behaviorComp.behavior->onUpdate(deltaTime);
		}
	}

	void Scene::updatePhysicsWorld(float deltaTime) {
		this->physicsWorld->update(deltaTime);
	}

	void Scene::updatePhysics(float timeInterpolationFactor) {

		// handle CollisionBodies
		{

			auto view = this->registry.view<CollisionBodyComponent, TransformComponent>();
			for (auto [entity, cbComp, transComp] : view.each()) {
				cbComp.collisionBody->setTransform(transComp.transform);

				std::optional<Entity> entityOptional = findEntityByCollisionBody(cbComp.collisionBody);
				
				if (!entityOptional.has_value()) {
					continue;
				}

				auto collidingRigidBodies = physicsWorld->getCollidingCollisionBodies(cbComp.collisionBody);

				for (auto collisionBody : collidingRigidBodies) {
					std::optional<Entity> otherEntityOptional = findEntityByCollisionBody(collisionBody);

					if (entityOptional.value().hasComponent<BehaviorComponent>()) {
						entityOptional.value().getComponent<BehaviorComponent>().behavior->onCollision(otherEntityOptional.value());
					}

				}
			}

		} 
		
		// handle RigidBodies
		{

			auto view = this->registry.view<RigidBodyComponent, TransformComponent>();

			for (auto [entity, rbComp, transComp] : view.each()) {
				if (rbComp.rigidBody->getRigidBodyType() == RigidBodyType::Static) {
					rbComp.rigidBody->setTransform(transComp.transform);
				}
				else {
					// perform transform interpolation
					Transform interpolatedTransform = rbComp.rigidBody->getInterpolatedTranform(timeInterpolationFactor);

					// update the transform component
					transComp.transform.position = interpolatedTransform.position;
					transComp.transform.rotation = interpolatedTransform.rotation;
				}
			}
		
		}

	}

	Camera& Scene::getCamera() {
		return this->camera;
	}

	entt::registry& Scene::getRegistry() {
		return this->registry;
	} 

	Entity Scene::createEntity() {
		entt::entity enttEntity = this->registry.create();
		Entity entity { enttEntity, this };
		return entity;
	}

	void Scene::destroyEntity(Entity& toBeDestroyedEntity) {
		this->registry.destroy(toBeDestroyedEntity.enttEntity);
	}

	std::optional<Entity> Scene::findEntityByCollisionBody(std::shared_ptr<CollisionBody> collisionBody) {
		auto view = this->registry.view<CollisionBodyComponent>();
		for (auto [entity, collisionBodyComponent] : view.each()) {
			if (collisionBodyComponent.collisionBody == collisionBody) {
				return { Entity{ entity, this } };
			}
		}
		
		return std::nullopt;
	}

	void Scene::onStaticMeshComponentCreated(entt::registry& registry, entt::entity entity) {

		auto& meshComp = registry.get<StaticMeshComponent>(entity);
		auto& materialComp = registry.get<MaterialComponent>(entity);
		auto& transComp = registry.get<TransformComponent>(entity);

		this->renderer3d.drawStaticMesh(
			meshComp.mesh,
			materialComp.material,
			transComp.transform.getTransformMatrix(),
			(uint32_t)entity);
	}

	void Scene::onRigidBodyComponentCreated(entt::registry& registry, entt::entity entity) {
		ASSERT(registry.has<TransformComponent>(entity),
			"Entiy must have a transform component to have a rigid body component");
		ASSERT(registry.has<CollisionShapeComponent>(entity),
			"Entiy must have a collision shape component to have a rigid body component");

		auto& transform = registry.get<TransformComponent>(entity).transform;
		auto& collisionShape = registry.get<CollisionShapeComponent>(entity).collisionShape;
		auto& rigidBodyComponent = registry.get<RigidBodyComponent>(entity);

		auto rigidBody = this->physicsWorld->createRigidBody(transform);
		rigidBody->addCollisionShape(transform, collisionShape);
		rigidBody->setRigidBodyType(rigidBodyComponent.type);

		rigidBodyComponent.rigidBody = rigidBody;
	}

	void Scene::onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity) {
		auto& rigidBodyComp = registry.get<RigidBodyComponent>(entity);
		
		physicsWorld->destroyRigidBody(rigidBodyComp.rigidBody);
	}

	void Scene::onCollisionBodyComponentCreated(entt::registry& registry, entt::entity entity) {

		ASSERT(registry.has<TransformComponent>(entity),
			"Entiy must have a transform component to have a rigid body component");
		ASSERT(registry.has<CollisionShapeComponent>(entity),
			"Entiy must have a collision shape component to have a rigid body component");

		auto& transform = registry.get<TransformComponent>(entity).transform;
		auto& collisionShape = registry.get<CollisionShapeComponent>(entity).collisionShape;
		auto& collisionBodyComponent = registry.get<CollisionBodyComponent>(entity);

		auto collisionBody = this->physicsWorld->createCollisionBody(transform);
		collisionBody->addCollisionShape(transform, collisionShape);

		collisionBodyComponent.collisionBody = collisionBody;

	}

	void Scene::onCollisionBodyComponentDestroyed(entt::registry& registry, entt::entity entity) {
		auto& collisionBodyComp = registry.get<CollisionBodyComponent>(entity);
		physicsWorld->destroyCollisionBody(collisionBodyComp.collisionBody);
	}

	void Scene::onBehaviorComponentDestroyed(entt::registry& registry, entt::entity entity) {
		auto& behaviorComponent = registry.get<BehaviorComponent>(entity);
		behaviorComponent.destroy();
	}

	void Scene::onStaticMeshComponentDestroyed(entt::registry& registry, entt::entity entity) {
		this->renderer3d.destroyStaticMesh((uint32_t) entity);
	}

	std::string Scene::generateUuid() {
		static std::random_device randomDevice;
		static std::mt19937 mt(randomDevice());

		std::uniform_int_distribution<int32_t> uniformIntDistribution(0, 15);

		const char* hexDigits = "0123456789abcdef";

		std::string uuid = "00000000-0000-0000-0000-000000000000";

		for (size_t i = 0; i < uuid.size(); i++) {
			if (uuid[i] != '-') {
				uuid[i] = hexDigits[uniformIntDistribution(mt)];
			}
		}

		return std::string(uuid);
	}
}