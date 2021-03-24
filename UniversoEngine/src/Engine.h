#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/Renderer3D.h"
#include "physics/PhysicsWorld.h"
#include "input/Input.h"
#include "scene/Scene.h"
#include "entt/entt.hpp"

#include "scene/Components.h"

#include "debug/Assert.h"

namespace engine {

	class Engine {

	private:
		const char* windowName;
		float windowWidth, windowHeight;
		GLFWwindow* window;
		Renderer3D* renderer;
		PhysicsWorld* physicsWorld;
		Scene *currentScene;

	public:
		static Engine& getInstance();

		Engine(Engine const&) = delete;

		void operator=(Engine const&) = delete;

		~Engine();

		void initiliaze(
			Scene* initialScene,
			float initialWindowWidth,
			float initialWindowHeight,
			const char* windowName);

		void run();

		void setScene(Scene* scene);

		void setViewPortSize(float newWindowWidth, float newWindowHeight);

	private:
		
		Engine();

		bool isRunning();

		void initializeCurrentScene();

		void updateCurrentSceneLogic(float deltaTime);

		void updateCurrentScenePhysics(float deltaTime);

		void renderCurrentScene(float deltaTime);

		void initializeGlfwWindow();
	};
}