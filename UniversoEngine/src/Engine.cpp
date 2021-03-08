#include "Engine.h"


namespace engine {

	Engine::Engine(
			Scene* initialScene,
			float windowWidth,
			float windowHeight,
			const char *windowName) {
		
		this->currentScene = initialScene;
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

		this->initializeCurrentScene();
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
		
		constexpr float fixedDeltaTime = 0.0169f; 

		this->renderCurrentScene(deltaTime);
		this->updateCurrentSceneLogic(fixedDeltaTime);
		this->updateCurrentScenePhysics(fixedDeltaTime);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glfwSwapBuffers(this->window);
		glfwPollEvents();
		
	}

	bool Engine::isRunning() {
		return !glfwWindowShouldClose(this->window);
	}

	void Engine::setScene(Scene* scene) {
		this->currentScene = scene;
		this->initializeCurrentScene();
	}

	void Engine::initializeCurrentScene() {

		this->currentScene->onStart();

		for (auto gameObject : this->currentScene->getGameObjects()) 
			gameObject->onStart();
	}

	void Engine::updateCurrentScenePhysics(float deltaTime) {
		auto view = this->currentScene->registry
		.view<RigidBodyComponent, TransformComponent>();

		for (auto [entity, rbComp, transComp]: view.each()) {
			this->physicsWorld->appendRigidBody(rbComp.rigidBody);
		} 
		
		this->physicsWorld->update(deltaTime);

		for (auto [entity, rbComp, transComp]: view.each()) {
			transComp.transform.position = rbComp.rigidBody->transform.position; 
		} 

		this->physicsWorld->clear();
	}

	void Engine::updateCurrentSceneLogic(float deltaTime) {
		this->currentScene->onUpdate(deltaTime);

		for (auto gameObject : this->currentScene->getGameObjects()) 
			gameObject->onUpdate(deltaTime);
	}

	void Engine::renderCurrentScene(float deltaTime) {
		auto view = this->currentScene->registry
		.view<MeshComponent, TextureComponent, TransformComponent>();

		auto mvp = currentScene->getCamera()->getMvp(this->windowWidth, windowHeight);
		this->rederer->clear(0.2f, 0.3f, 0.3f, 1.0f);
		this->rederer->startDrawing(mvp);

		for (auto [entity, meshComp, textComp, transComp] : view.each()) {
			this->rederer->drawMesh(
				meshComp.mesh, 
				textComp.texture,
				transComp.transform.position,
				transComp.transform.scale,
				transComp.transform.rotationAxis,
				transComp.transform.rotationAngle
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
		// glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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