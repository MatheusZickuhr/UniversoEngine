#include "Engine.h"


namespace engine {

	Engine::Engine(float windowWidth, float windowHeight, const char* windowName) {
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

	void Engine::run() {
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

	std::shared_ptr<LevelLoadingManager> Engine::getLevelLoadingManager() {
		return this->levelLoadingManager;
	}

	void Engine::renderAndUpdateCurrentLevel(float deltaTime) {
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

	void Engine::initializeGlfwWindow() {
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

	void Engine::checkGlad() {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
#ifdef _DEBUG
			__debugbreak();
#endif
		}

	}

}