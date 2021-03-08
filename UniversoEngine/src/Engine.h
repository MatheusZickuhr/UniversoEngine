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

namespace engine {

	class Engine {

	private:
		float windowWidth, windowHeight, lastFrameTime;
		const char *windowName;
		GLFWwindow *window;
		Renderer3D* rederer;
		PhysicsWorld* physicsWorld;
		Scene *currentScene;

	public:
		Engine(
			Scene* initialScene,
			float windowWidth,
			float windowHeight,
			const char *windowName);

		~Engine();

		void tick();

		bool isRunning();

		void setScene(Scene* scene);

	private:
	
		void initializeCurrentScene();

		void updateCurrentSceneLogic(float deltaTime);

		void updateCurrentScenePhysics(float deltaTime);

		void renderCurrentScene(float deltaTime);

		void initializeGlfwWindow();

		void checkGlad();
	};
}