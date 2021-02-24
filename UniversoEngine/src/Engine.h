#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/Renderer3D.h"
#include "physics/PhysicsWorld.h"
#include "input/Input.h"
#include "level/Level.h"

namespace engine {

	class Engine {

	private:
		float windowWidth, windowHeight, lastFrameTime;
		const char *windowName;
		GLFWwindow *window;
		Renderer3D* rederer;
		PhysicsWorld* physicsWorld;
		Level *currentLevel;

	public:
		Engine(
			Level* initialLevel,
			float windowWidth,
			float windowHeight,
			const char *windowName);

		~Engine();

		void tick();

		bool isRunning();

		void setLevel(Level* level);

	private:
	
		void initializeCurrentLevel();

		void updateCurrentLevelLogic(float deltaTime);

		void updateCurrentLevelPhysics(float deltaTime);

		void renderCurrentLevel(float deltaTime);

		void initializeGlfwWindow();

		void checkGlad();
	};
}