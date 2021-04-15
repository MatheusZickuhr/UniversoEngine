#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine {

	void framebufferSizeCallback(GLFWwindow* window, int newWindowWidth, int newWindowHeight) {
		Application::getInstance().setViewPortSize(newWindowWidth, newWindowHeight);
	}

	Application& Application::getInstance() {
		static Application instance;
		return instance;
	}

	Application::Application() : windowWidth(800), windowHeight(600), windowName("Universo Application"),
		window(nullptr), currentScene(nullptr) {}

	Application::~Application() {
		delete this->currentScene;
		glfwTerminate();
	}

	void Application::setViewPortSize(float newWindowWidth, float newWindowHeight) {
		this->windowWidth = newWindowWidth;
		this->windowHeight = newWindowHeight;
		this->currentScene->getRenderer()->setViewPortSize(newWindowWidth, newWindowHeight);
	}

	void Application::run() {
		constexpr float fixedDeltaTime = 1.0f / 60.0f;
		
		float lastTime = 0.0f;

		float accumulator = 0.0f;

		while (this->isRunning()) {
			float currentTime = glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// update the physics world
			accumulator += deltaTime;

			while (accumulator >= fixedDeltaTime) {
				this->currentScene->updatePhysicsWorld(fixedDeltaTime);
				accumulator -= fixedDeltaTime;
			}

			// update the current scene with the updated physics world data
			float timeInterpolationFactor = accumulator / fixedDeltaTime;
			this->currentScene->updatePhysics(timeInterpolationFactor);
			
			// update the current scene runtime
			this->currentScene->onUpdateCallBack(deltaTime);

			// in the end just render the current scene
			this->currentScene->render(this->windowWidth, this->windowHeight);

#ifdef DEBUG
			this->currentScene->renderDebugData();
#endif

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}

	}

	bool Application::isRunning() {
		return !glfwWindowShouldClose(this->window);
	}

	void Application::initializeGlfwWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
		this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->windowName, NULL, NULL);

		ASSERT(this->window != NULL, "Failed to create GLFW window");
		
		glfwMakeContextCurrent(this->window);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(0);

		const bool gladLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		
		ASSERT(gladLoaded, "Failed to initialize GLAD");
	}

	void Application::initializeImGui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");

	}

}