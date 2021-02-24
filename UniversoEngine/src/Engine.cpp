#include "Engine.h"


namespace engine {

	Engine::Engine(
			Level* initialLevel,
			float windowWidth,
			float windowHeight,
			const char *windowName) {
		
		this->currentLevel = initialLevel;
		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;
		this->windowName = windowName;
		this->lastFrameTime = 0.0f; 

		this->initializeGlfwWindow();
		this->checkGlad();
		glViewport(0, 0, windowWidth, windowHeight);

		this->rederer = new Renderer3D();

		this->physicsWorld = new PhysicsWorld();

		Input::init(this->window);

		this->currentLevel->start();
	}

	Engine::~Engine() {
		delete this->rederer;
		delete this->physicsWorld;
		glfwTerminate();
	}

	void Engine::tick() {
		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - this->lastFrameTime;
		this->lastFrameTime = currentFrameTime;

		this->renderCurrentLevel(deltaTime);
		this->updateCurrentLevelLogic(deltaTime);
		this->updateCurrentLevelPhysics(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glfwSwapBuffers(this->window);
		glfwPollEvents();
		
	}

	bool Engine::isRunning() {
		return !glfwWindowShouldClose(this->window);
	}

	void Engine::setLevel(Level* level) {
		this->currentLevel = level;
		this->currentLevel->start();
	}

	void Engine::updateCurrentLevelPhysics(float deltaTime) {
		for (auto gameObject : currentLevel->getGameObjects()) {
			gameObject->rigidBody->position = gameObject->transform->position; 
			this->physicsWorld->appendRigidBody(gameObject->rigidBody);
		}

		this->physicsWorld->update(deltaTime);

		for (auto gameObject : currentLevel->getGameObjects()) {
			 gameObject->transform->position = gameObject->rigidBody->position;
		}

		this->physicsWorld->clear();

	}

	void Engine::updateCurrentLevelLogic(float deltaTime) {
		this->currentLevel->update(deltaTime);
	}

	void Engine::renderCurrentLevel(float deltaTime) {
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