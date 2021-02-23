#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/Renderer3D.h"
#include "Input.h"
#include "level/LevelLoadingManager.h"

namespace engine {

	class Engine {

	private:
		float windowWidth, windowHeight;
		const char* windowName;
		GLFWwindow* window;
		std::unique_ptr<Renderer3D> rederer;
		std::shared_ptr<LevelLoadingManager> levelLoadingManager;
		
	public:

		Engine(float windowWidth, float windowHeight, const char* windowName);
		
		void run();

		std::shared_ptr<LevelLoadingManager> getLevelLoadingManager();;

	private:

		void renderAndUpdateCurrentLevel(float deltaTime);

		void initializeGlfwWindow();

		void checkGlad();
	};
}