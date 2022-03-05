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

		this->registry.on_destroy<BehaviorComponent>()
			.connect<&Scene::onBehaviorComponentDestroyed>(this);

		this->registry.on_destroy<StaticMeshComponent>()
			.connect<&Scene::onStaticMeshComponentDestroyed>(this);
	}

	Scene::~Scene() {
		delete this->physicsWorld;

		for (Entity* entity : entities) {
			delete entity;
		}
		entities.clear();
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
						(unsigned int)entity);
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

		auto view = this->registry.view<RigidBodyComponent, TransformComponent>();
		
		for (auto [entity, rbComp, transComp] : view.each()) {
			// perform transform interpolation
			Transform interpolatedTransform = rbComp.rigidBody->getInterpolatedTranform(timeInterpolationFactor);

			// update the transform component
			transComp.transform.position = interpolatedTransform.position;
			transComp.transform.rotation = interpolatedTransform.rotation;
		}
	}

	Camera& Scene::getCamera() {
		return this->camera;
	}

	entt::registry& Scene::getRegistry() {
		return this->registry;
	} 

	Entity* Scene::createEntity() {
		entt::entity enttEntity = this->registry.create();
		Entity* entity = new Entity{ enttEntity, this };
		this->entities.push_back(entity);
		return entity;
	}

	void Scene::destroyEntity(Entity* toBeDestroyedEntity) {
		int entityIndex = 0;
		bool entityFound = false;

		for (Entity* entity : this->entities) {
			if (toBeDestroyedEntity == entity) {
				entityFound = true;
				break;
			}
			entityIndex++;
		}

		if (entityFound) {
			this->entities.erase(this->entities.begin() + entityIndex);
			this->registry.destroy(toBeDestroyedEntity->enttEntity);
		}
	}

	void Scene::onStaticMeshComponentCreated(entt::registry& registry, entt::entity entity) {

		auto& meshComp = registry.get<StaticMeshComponent>(entity);
		auto& materialComp = registry.get<MaterialComponent>(entity);
		auto& transComp = registry.get<TransformComponent>(entity);

		this->renderer3d.drawStaticMesh(
			meshComp.mesh,
			materialComp.material,
			transComp.transform.getTransformMatrix(),
			(unsigned int)entity);
	}

	void Scene::onRigidBodyComponentCreated(entt::registry& registry, entt::entity entity) {
		ASSERT(registry.has<TransformComponent>(entity),
			"Entiy must have a transform component to have a rigid body component");
		ASSERT(registry.has<CollisionShapeComponent>(entity),
			"Entiy must have a collision shape component to have a rigid body component");

		auto& transform = registry.get<TransformComponent>(entity).transform;
		auto& collisionShape = registry.get<CollisionShapeComponent>(entity).collisionShape;
		auto& rigidBodyComponent = registry.get<RigidBodyComponent>(entity);

		//create the RigidBody
		RigidBody* rigidBody = this->physicsWorld->createRigidBody(transform.position, transform.rotation);

		// Create the collisionShape for the RigidBody
		rigidBody->addCollisionShape(transform.scale, collisionShape);

		// set the RigidBody type
		rigidBody->setRigidBodyType(rigidBodyComponent.type);

		// update the RigidBodyComponent with the created RigidBody
		rigidBodyComponent.rigidBody = rigidBody;
	}

	void Scene::onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity) {
		auto& rigidBodyComp = registry.get<RigidBodyComponent>(entity);
		
		physicsWorld->destroyRigidBody(rigidBodyComp.rigidBody);
	}

	void Scene::onBehaviorComponentDestroyed(entt::registry& registry, entt::entity entity) {
		auto& behaviorComponent = registry.get<BehaviorComponent>(entity);
		behaviorComponent.destroy();
	}

	void Scene::onStaticMeshComponentDestroyed(entt::registry& registry, entt::entity entity) {
		this->renderer3d.destroyStaticMesh((unsigned int) entity);
	}

	std::string Scene::generateUuid() {
		static std::random_device randomDevice;
		static std::mt19937 mt(randomDevice());

		std::uniform_int_distribution<int> uniformIntDistribution(0, 15);

		const char* hexDigits = "0123456789abcdef";

		std::string uuid = "00000000-0000-0000-0000-000000000000";

		for (int i = 0; i < uuid.size(); i++) {
			if (uuid[i] != '-') {
				uuid[i] = hexDigits[uniformIntDistribution(mt)];
			}
		}

		return std::string(uuid);
	}
}