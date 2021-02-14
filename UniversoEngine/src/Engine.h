#pragma once

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
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

		Engine(float windowWidth, float windowHeight, const char* windowName);
		
		void run();

		std::shared_ptr<LevelLoadingManager> getLevelLoadingManager();;

	private:

		void renderAndUpdateCurrentLevel(float deltaTime);

		void initializeGlfwWindow();

		void checkGlad();
	};
}