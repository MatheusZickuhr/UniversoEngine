#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "../debug/Assert.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine {

	Scene::Scene() :
		physicsWorld(new ReactPhysics3dPhysicsWorld()),
		renderer3d(new Renderer3D()),
		renderer2d(new Renderer2D()) {
	
		this->registry.on_construct<RigidBodyComponent>()
			.connect<&Scene::onRigidBodyComponentCreated>(this);

		this->registry.on_destroy<RigidBodyComponent>()
			.connect<&Scene::onRigidBodyComponentDestroyed>(this);

		this->registry.on_destroy<BehaviorComponent>()
			.connect<&Scene::onBehaviorComponentDestroyed>(this);
	}

	Scene::~Scene() {
		delete this->renderer3d;
		delete this->renderer2d;
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

		this->renderer3d->clearColor(0.0f, 0.0f, 0.0f, 1.0f);

		this->renderer3d->startLightsDrawing();
			// draw/ update lights
			{
				auto view = this->registry.view<PointLightComponent, TransformComponent>();

				for (auto [entity, lightComp, transComp] : view.each()) {
					this->renderer3d->drawPointLight(lightComp.pointLight, transComp.transform.getTransformMatrix());
				}
			}

			{
				auto view = this->registry.view<DirectionalLightComponent, TransformComponent>();

				for (auto [entity, lightComp, transComp] : view.each()) {
					this->renderer3d->drawDirectionalLight(lightComp.directionalLight, this->camera, transComp.transform.getTransformMatrix());
				}
			}
			// end update/draw lights

			{
				auto view = this->registry.view<MeshComponent, MaterialComponent, TransformComponent>();

				for (auto [entity, meshComp, materialComp, transComp] : view.each()) {
					this->renderer3d->drawMeshShadowMap(meshComp.mesh, transComp.transform.getTransformMatrix());
				}
			}
		this->renderer3d->endLightsDrawing();

		this->renderer3d->startDrawing(this->camera);
			{
				auto view = this->registry.view<MeshComponent, MaterialComponent, TransformComponent>();

				for (auto [entity, meshComp, materialComp, transComp] : view.each()) {
					this->renderer3d->drawMesh(meshComp.mesh, materialComp.material, transComp.transform.getTransformMatrix());
				}
			}
		this->renderer3d->endDrawing();
	}

	void Scene::renderDebugData() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug data", 0, ImGuiWindowFlags_AlwaysAutoResize);
	
		ImGui::Text("Frametime: %.1f", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("Fps: %.1f", ImGui::GetIO().Framerate);

		ImGui::Text("Draw calls: %d", this->renderer3d->getDrawCallsCount());

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Scene::renderDebugLightPositions() {
		auto view = this->registry.view<PointLightComponent, TransformComponent>();

		this->renderer2d->startDrawing(this->camera);

		for (auto [entity, lightComp, transComp] : view.each()) {
			this->renderer2d->drawQuad(&this->debugPointLightTexture, transComp.transform.getTransformMatrix());
		}

		this->renderer2d->endDrawing();
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

	Renderer3D* Scene::getRenderer() {
		return this->renderer3d;
	}

	Entity* Scene::createEntity() {
		entt::entity enttEntity = this->registry.create();
		Entity* entity = new Entity{ enttEntity, this };
		this->entities.push_back(entity);
		return entity;
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

	}

	void Scene::onBehaviorComponentDestroyed(entt::registry& registry, entt::entity entity) {
		auto& behaviorComponent = registry.get<BehaviorComponent>(entity);
		behaviorComponent.destroy();
	}
}