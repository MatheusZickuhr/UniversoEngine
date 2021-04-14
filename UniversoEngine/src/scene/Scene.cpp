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
		renderer(new Renderer3D()) {
	
		this->registry.on_construct<RigidBodyComponent>()
			.connect<&Scene::onRigidBodyComponentCreated>(this);

		this->registry.on_destroy<RigidBodyComponent>()
			.connect<&Scene::onRigidBodyComponentDestroyed>(this);

		this->registry.on_destroy<BehaviorComponent>()
			.connect<&Scene::onBehaviorComponentDestroyed>(this);
	}

	Scene::~Scene() {
		delete this->renderer;
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

	void Scene::render(float windowWidth, float windowHeight) {
		auto view = this->registry.view<MeshComponent, TextureComponent, TransformComponent>();

		auto mvp = this->camera.getMvp(windowWidth, windowHeight);
		this->renderer->clear(0.2f, 0.3f, 0.3f, 1.0f);
		this->renderer->startDrawing(mvp);

		for (auto [entity, meshComp, textComp, transComp] : view.each()) {
			this->renderer->drawMesh(meshComp.mesh, textComp.texture, transComp.transform.getTransformMatrix());
		}
		this->renderer->endDrawing();
	}

	void Scene::renderDebugData() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug data", false, ImGuiWindowFlags_AlwaysAutoResize);
	
		ImGui::Text("Frametime: %.1f", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("Fps: %.1f", ImGui::GetIO().Framerate);

		ImGui::Text("Draw calls: %d", this->renderer->getDrawCallsCount());

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
		return this->renderer;
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