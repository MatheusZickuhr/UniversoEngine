#pragma once

#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer/Renderer3D.h"
#include "input/CameraKeybordAndMouseInput.h"
#include "Level.h"

namespace engine {

	class Engine {

	private:
		float windowWidth, windowHeight;
		const char* windowName;
		GLFWwindow* window;
		std::unique_ptr<Renderer3D> rederer;
		std::shared_ptr<Camera> camera;
		std::unique_ptr<CameraKeybordAndMouseInput> cameraInput;
		std::unique_ptr<Level> currentLevel;
		
	public:

		Engine(float windowWidth, float windowHeight, const char* windowName) {
			this->windowWidth = windowWidth;
			this->windowHeight = windowHeight;
			this->windowName = windowName;

			this->initializeGlfwWindow();
			this->checkGlad();
			glViewport(0, 0, windowWidth, windowHeight);

			this->rederer = std::make_unique<Renderer3D>();
			this->camera = std::make_shared<Camera>(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 3.0f));
			this->cameraInput = std::make_unique<CameraKeybordAndMouseInput>(camera);
		}
		
		void run() {
			float deltaTime = 0.0f;
			float lastFrame = 0.0f;

			while (!glfwWindowShouldClose(this->window)) {
				float currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;

				this->checkIfCurrentLevelIsLoaded();

				this->cameraInput->update(this->window, deltaTime);

				this->renderAndUpdateCurrentLevel(deltaTime);

				glfwSwapBuffers(this->window);
				glfwPollEvents();
			}

			glfwTerminate();
		}

		template <typename T>
		void loadLevel() {
			this->currentLevel = std::make_unique<T>();
			this->currentLevel->setEngineInstance(this);
			this->currentLevel->start();
		}

		std::shared_ptr<Camera> getCamera() {
			return this->camera;
		}


	private:

		void renderAndUpdateCurrentLevel(float deltaTime) {
			this->rederer->clear(0.2f, 0.3f, 0.3f, 1.0f);
			this->rederer->startDrawing(this->camera);
			for (auto gameObject : this->currentLevel->getGameObjects()) {
				this->rederer->drawGameObject(gameObject);
			}
			this->rederer->endDrawing();

			this->currentLevel->update(deltaTime);
		}

		void checkIfCurrentLevelIsLoaded() {
			if (this->currentLevel == nullptr) {

				std::cout << "The engine requires a inicial level" << std::endl;
#ifdef _DEBUG
				__debugbreak();
#endif
			}
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