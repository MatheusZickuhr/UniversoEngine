#include "Window.h"
#include "debug/Assert.h"
#include "renderer/renderer_api/DrawApi.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace engine {

	void framebufferSizeCallback(GLFWwindow* window, int32_t newWindowWidth, int32_t newWindowHeight);

	void initializeImGui(GLFWwindow* glfwWindow);

	Window::Window(const char* name, int32_t width, int32_t height)
		: name(name), width(width), height(height), glfwWindow(nullptr) {

		initializeGlfwWindow();
		initializeImGui(glfwWindow);
	}

	Window::~Window() {
		std::cout << "Window destructor" << std::endl;
		glfwTerminate();
	}

	bool Window::isRunning() {
		return !glfwWindowShouldClose(this->glfwWindow);
	}

	void Window::initializeGlfwWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		#endif

		this->glfwWindow = glfwCreateWindow(this->width, this->height, this->name, NULL, NULL);

		ASSERT(this->glfwWindow != NULL, "Failed to create GLFW window");

		glfwMakeContextCurrent(this->glfwWindow);
		glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);
		glfwSetInputMode(this->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(1);

		const bool gladLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		ASSERT(gladLoaded, "Failed to initialize GLAD");
	}

	void Window::swapBuffers() {
		glfwSwapBuffers(this->glfwWindow);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

	void Window::close() {
		glfwSetWindowShouldClose(glfwWindow, true);
	}


	void Window::getCursorPos(float* xpos, float* ypos) {
		double xposAsdouble, yposAsDouble;
		glfwGetCursorPos(this->glfwWindow, &xposAsdouble, &yposAsDouble);
		*xpos = (float)xposAsdouble;
		*ypos = (float)yposAsDouble;
	}

	bool Window::keyPressed(int32_t key) {
		return glfwGetKey(this->glfwWindow, key) == GLFW_PRESS;
	}

	bool Window::mouseButtonPressed(int32_t key) {
		return glfwGetMouseButton(this->glfwWindow, key) == GLFW_PRESS;
	}

	void framebufferSizeCallback(GLFWwindow* window, int32_t newWindowWidth, int32_t newWindowHeight) {
		DrawApi::setViewPortSize(newWindowWidth, newWindowHeight);
	}

	void initializeImGui(GLFWwindow* glfwWindow) {
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
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);

		ImGui_ImplOpenGL3_Init("#version 440 core");
	}

}