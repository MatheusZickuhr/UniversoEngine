#pragma once

#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer/Renderer3D.h"
#include "Input.h"
#include "Level.h"
#include "LevelManager.h"


namespace engine {

	class Engine {

	private:
		float windowWidth, windowHeight;
		const char* windowName;
		GLFWwindow* window;
		std::unique_ptr<Renderer3D> rederer;
		std::shared_ptr<LevelManager> levelManager;
		
	public:

		Engine(float windowWidth, float windowHeight, const char* windowName) {
			this->windowWidth = windowWidth;
			this->windowHeight = windowHeight;
			this->windowName = windowName;

			this->initializeGlfwWindow();
			this->checkGlad();
			glViewport(0, 0, windowWidth, windowHeight);

			this->levelManager = std::make_shared<LevelManager>();
			this->rederer = std::make_unique<Renderer3D>();
			
			Input::init(this->window);
		}
		
		void run() {
			float deltaTime = 0.0f;
			float lastFrame = 0.0f;

			while (!glfwWindowShouldClose(this->window)) {
				float currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;

				this->renderAndUpdateCurrentLevel(deltaTime);

				if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
					glfwSetWindowShouldClose(window, true);

				glfwSwapBuffers(this->window);
				glfwPollEvents();
			}

			glfwTerminate();
		}

		std::shared_ptr<LevelManager> getLevelManager() {
			return this->levelManager;
		}

	private:

		void renderAndUpdateCurrentLevel(float deltaTime) {
			auto currentLevel = this->levelManager->getCurrentLevel();
			auto mvp = currentLevel->getCamera()->getMvp(this->windowWidth, windowHeight);

			this->rederer->clear(0.2f, 0.3f, 0.3f, 1.0f);
			this->rederer->startDrawing(mvp);
			for (auto gameObject : currentLevel->getGameObjects()) {
				this->rederer->drawMesh(gameObject->mesh,
					gameObject->texture,
					gameObject->scale,
					gameObject->position);
			}
			this->rederer->endDrawing();

			currentLevel->update(deltaTime);
		}

		void initializeGlfwWindow() {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

			this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->windowName, NULL, NULL);

			if (this->window == NULL) {
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
#ifdef _DEBUG
				__debugbreak();
#endif
			}

			glfwMakeContextCurrent(this->window);
			glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} 

		void checkGlad() {
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cout << "Failed to initialize GLAD" << std::endl;
#ifdef _DEBUG
				__debugbreak();
#endif
			}

		}
	};
}