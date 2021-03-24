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
		window(nullptr), currentScene(nullptr), renderer(nullptr), physicsWorld(nullptr)  {}

	void Engine::initiliaze(
		Scene* initialScene,
		float initialWindowWidth,
		float initialWindowHeight,
		const char* windowName) {
		

		this->windowWidth = initialWindowWidth;
		this->windowHeight = initialWindowHeight;

		this->currentScene = initialScene;
		this->windowName = windowName;

		this->initializeGlfwWindow();

		this->setViewPortSize(this->windowWidth, this->windowHeight);

		this->renderer = new Renderer3D();

		this->physicsWorld = new PhysicsWorld();

		Input::init(this->window);

		this->initializeCurrentScene();
		
	}

	Engine::~Engine() {
		delete this->renderer;
		delete this->physicsWorld;
		glfwTerminate();
	}

	void Engine::setViewPortSize(float newWindowWidth, float newWindowHeight) {
		this->windowWidth = newWindowWidth;
		this->windowHeight = newWindowHeight;
		glViewport(0, 0, newWindowWidth, newWindowHeight);
	}


	void Engine::run() {
		constexpr float fixedDeltaTime = 1.0f / 60.0f;
		
		float lastFrameTime = 0.0f;

		while (this->isRunning()) {
			float currentFrameTime = glfwGetTime();
			float deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			this->renderCurrentScene(deltaTime);
			this->updateCurrentSceneLogic(fixedDeltaTime);
			this->updateCurrentScenePhysics(fixedDeltaTime);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}
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

		auto view = this->currentScene->registry.view<BehaviorComponent>();
			for (auto [entity, behaviorComp]: view.each()) {
			behaviorComp.behavior->initialize();                                 
			behaviorComp.behavior->onStart();
		} 
	}

	void Engine::updateCurrentScenePhysics(float deltaTime) {
		auto view = this->currentScene->registry
		.view<RigidBodyComponent, TransformComponent>();

		for (auto [entity, rbComp, transComp] : view.each()) {
			rbComp.rigidBody->transform = &transComp.transform;
			this->physicsWorld->appendRigidBody(rbComp.rigidBody.get());
		}

		this->physicsWorld->update(deltaTime);
		this->physicsWorld->clear();
	}

	void Engine::updateCurrentSceneLogic(float deltaTime) {
		this->currentScene->onUpdate(deltaTime);

		auto view = this->currentScene->registry.view<BehaviorComponent>();

		for (auto [entity, behaviorComp]: view.each()) {

			if (!behaviorComp.behavior->isInitialized()) {
				behaviorComp.behavior->initialize();
				behaviorComp.behavior->onStart();
			}

			behaviorComp.behavior->onUpdate(deltaTime);
		} 

	}

	void Engine::renderCurrentScene(float deltaTime) {
		auto view = this->currentScene->registry
		.view<MeshComponent, TextureComponent, TransformComponent>();

		auto mvp = currentScene->getCamera()->getMvp(this->windowWidth, this->windowHeight);
		this->renderer->clear(0.2f, 0.3f, 0.3f, 1.0f);
		this->renderer->startDrawing(mvp);

		for (auto [entity, meshComp, textComp, transComp] : view.each()) {
			this->renderer->drawMesh(
				meshComp.mesh, 
				textComp.texture,
				transComp.transform.position,
				transComp.transform.scale,
				transComp.transform.rotationAxis,
				transComp.transform.rotationAngle
			);
		}
		this->renderer->endDrawing();
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