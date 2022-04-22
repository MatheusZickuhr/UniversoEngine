#include "Application.h"
#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <algorithm>

namespace engine {

	void framebufferSizeCallback(GLFWwindow* window, int32_t newWindowWidth, int32_t newWindowHeight) {
		DrawApi::setViewPortSize(newWindowWidth, newWindowHeight);
	}

	Application::Application() : windowWidth(800), windowHeight(600), windowName("Universo Application"),
		window(nullptr), currentScene(nullptr) {}

	void Application::run() {
		constexpr float fixedDeltaTime = 1.0f / 60.0f;
		// the simulation will run slower after this framerate/deltatime limit 
		constexpr float maxDeltaTime = 1.0f / 10.0f;
		
		float lastTime = 0.0f;

		float accumulator = 0.0f;

		while (this->isRunning()) {
			float currentTime = (float)glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// check if the scene changed during runtime
			if (nextScene) {
				currentScene = std::move(nextScene);
				currentScene->initialize(physicsWorld, renderer3d, renderer2d);
			}

			deltaTime = std::min(deltaTime, maxDeltaTime);

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
			DrawApi::clearDepthAndColorBuffer(); // clean the buffers in the main frame buffer
			this->currentScene->render();
			this->onRender();

			// imgui rendering
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			this->onImGuiRender();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// enable viewports
			//GLFWwindow* backup_current_context = glfwGetCurrentContext();
			//ImGui::UpdatePlatformWindows();
			//ImGui::RenderPlatformWindowsDefault();
			//glfwMakeContextCurrent(backup_current_context);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}

		glfwTerminate();
	}

	void Application::onImGuiRender() {

	}

	void Application::onRender() {
	}

	void Application::onInitialize() {
	}

	bool Application::isRunning() {
		return !glfwWindowShouldClose(this->window);
	}

	void Application::initializeGlfwWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
		this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->windowName, NULL, NULL);

		ASSERT(this->window != NULL, "Failed to create GLFW window");
		
		glfwMakeContextCurrent(this->window);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(1);

		const bool gladLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		
		ASSERT(gladLoaded, "Failed to initialize GLAD");
	}

	void Application::initializeImGui() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		
		ImGui_ImplOpenGL3_Init("#version 440 core");

	}

}