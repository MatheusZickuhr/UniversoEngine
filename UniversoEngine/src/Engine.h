#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "renderer/Renderer3D.h"
#include "input/Input.h"
#include "scene/Scene.h"
#include "scene/Components.h"
#include "physics/RigidBody.h"
#include "physics/ReactPhysics3dPhysicsWorld.h"
#include "debug/Assert.h"

namespace engine {

	class Engine {

	private:
		const char* windowName;
		float windowWidth, windowHeight;
		GLFWwindow* window;
		Renderer3D* renderer;
		Scene *currentScene;
		PhysicsWorld* physicsWorld;

	public:
		static Engine& getInstance();

		Engine(Engine const&) = delete;

		void operator=(Engine const&) = delete;

		~Engine();

		void initiliaze(Scene* scene, float width, float height, const char* windowName);

		void run();

		void setScene(Scene* scene);

		void setViewPortSize(float newWindowWidth, float newWindowHeight);

	private:
		
		Engine();

		bool isRunning();

		void onRigidBodyComponentCreated(entt::registry& registry, entt::entity entity);

		void onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity);

		void initializeCurrentScene();

		void renderCurrentScene();

		void updateCurrentScenePhysics(float timeInterpolationFactor);

		void initializeGlfwWindow();
	};
}