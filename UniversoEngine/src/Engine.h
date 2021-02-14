#pragma once

#define STB_IMAGE_IMPLEMENTATION

// c++ std
#include <stdio.h>
#include <iostream>
#include <memory>
#include <map>
#include <array>
#include <fstream>
#include <string>
#include <vector>

// third party libraries
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OBJ_Loader/OBJ_Loader.h"

// engine
#include "renderer/renderer_api/VertexArray.h"
#include "renderer/renderer_api/VertexBuffer.h"
#include "renderer/renderer_api/IndexBuffer.h"
#include "renderer/renderer_api/Shader.h"
#include "renderer/renderer_api/ShaderProgram.h"
#include "renderer/renderer_api/Texture.h"
#include "renderer/renderer_api/Drawer.h"
#include "renderer/Vertex.h"
#include "renderer/Mesh.h"
#include "renderer/Camera.h"
#include "renderer/Renderer3D.h"
#include "Transform.h"
#include "GameObject.h"
#include "Input.h"
#include "Level.h"
#include "LevelLoadingManager.h"


namespace engine {

	class Engine {

	private:
		float windowWidth, windowHeight;
		const char* windowName;
		GLFWwindow* window;
		std::unique_ptr<Renderer3D> rederer;
		std::shared_ptr<LevelLoadingManager> levelLoadingManager;
		
	public:

		Engine(float windowWidth, float windowHeight, const char* windowName) {
			this->windowWidth = windowWidth;
			this->windowHeight = windowHeight;
			this->windowName = windowName;

			this->initializeGlfwWindow();
			this->checkGlad();
			glViewport(0, 0, windowWidth, windowHeight);

			this->levelLoadingManager = std::make_shared<LevelLoadingManager>();
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

		std::shared_ptr<LevelLoadingManager> getLevelLoadingManager() {
			return this->levelLoadingManager;
		}

	private:

		void renderAndUpdateCurrentLevel(float deltaTime) {
			auto currentLevel = this->levelLoadingManager->getCurrentLevel();
			auto mvp = currentLevel->getCamera()->getMvp(this->windowWidth, windowHeight);

			this->rederer->clear(0.2f, 0.3f, 0.3f, 1.0f);
			this->rederer->startDrawing(mvp);
			for (auto gameObject : currentLevel->getGameObjects()) {
				this->rederer->drawMesh(
					gameObject->mesh,
					gameObject->texture,
					gameObject->transform->position,
					gameObject->transform->scale,
					gameObject->transform->rotationAxis,
					gameObject->transform->rotationAngle
				);
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