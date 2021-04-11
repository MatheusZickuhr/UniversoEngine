#include "Engine.h"


namespace engine {

	void framebufferSizeCallback(GLFWwindow* window, int newWindowWidth, int newWindowHeight) {
		Engine::getInstance().setViewPortSize(newWindowWidth, newWindowHeight);
	}

	Engine& Engine::getInstance() {
		static Engine instance;
		return instance;
	}

	Engine::Engine() : windowWidth(800), windowHeight(600), windowName("Universo Engine"),
		window(nullptr), currentScene(nullptr) {}

	Engine::~Engine() {
		delete this->currentScene;
		glfwTerminate();
	}

	void Engine::setViewPortSize(float newWindowWidth, float newWindowHeight) {
		this->windowWidth = newWindowWidth;
		this->windowHeight = newWindowHeight;
		this->currentScene->getRenderer()->setViewPortSize(newWindowWidth, newWindowHeight);
	}

	void Engine::run() {
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
		
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}
	}

	bool Engine::isRunning() {
		return !glfwWindowShouldClose(this->window);
	}

	void Engine::initializeGlfwWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->windowName, NULL, NULL);

		ASSERT(this->window != NULL, "Failed to create GLFW window");
		
		glfwMakeContextCurrent(this->window);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(1);

		ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");
	}

}