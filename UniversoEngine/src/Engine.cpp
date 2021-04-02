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

	void Engine::initiliaze(Scene* scene, float width, float height, const char* windowName) {

		this->windowWidth = width;
		this->windowHeight = height;
		this->windowName = windowName;
		this->initializeGlfwWindow();
		
		this->renderer = new Renderer3D();
		this->physicsWorld = new PhysicsWorld();
		Input::init(this->window);
		
		this->currentScene = scene;
		this->initializeCurrentScene();

		this->setViewPortSize(this->windowWidth, this->windowHeight);
	}

	Engine::~Engine() {
		delete this->renderer;
		delete this->physicsWorld;
		glfwTerminate();
	}

	void Engine::setViewPortSize(float newWindowWidth, float newWindowHeight) {
		this->windowWidth = newWindowWidth;
		this->windowHeight = newWindowHeight;
		this->renderer->setViewPortSize(newWindowWidth, newWindowHeight);
	}

	void Engine::run() {
		constexpr float fixedDeltaTime = 1.0f / 60.0f;
		
		float lastFrameTime = 0.0f;

		while (this->isRunning()) {
			float currentFrameTime = glfwGetTime();
			float deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			this->renderCurrentScene(deltaTime);
			this->physicsWorld->update(fixedDeltaTime);
			this->currentScene->update(fixedDeltaTime);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}
	}

	bool Engine::isRunning() {
		return !glfwWindowShouldClose(this->window);
	}

	void Engine::onRigidBodyComponentCreated(entt::registry& registry, entt::entity entity) {
		auto rbComp = registry.get<RigidBodyComponent>(entity);
		this->physicsWorld->appendRigidBody(rbComp.rigidBody.get());
	}

	void Engine::onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity) {
		auto rbComp = registry.get<RigidBodyComponent>(entity);
		this->physicsWorld->removeRigidBody(rbComp.rigidBody.get());
	}

	void Engine::initializeCurrentScene() {
		this->currentScene->getRegistry().on_construct<RigidBodyComponent>()
			.connect<&Engine::onRigidBodyComponentCreated>(this);

		this->currentScene->getRegistry().on_destroy<RigidBodyComponent>()
			.connect<&Engine::onRigidBodyComponentDestroyed>(this);

		this->currentScene->start();
	}

	void Engine::setScene(Scene* scene) {
		this->currentScene = scene;
		this->initializeCurrentScene();
	}

	void Engine::renderCurrentScene(float deltaTime) {
		auto view = this->currentScene->getRegistry()
		.view<MeshComponent, TextureComponent, TransformComponent>();

		auto mvp = currentScene->getCamera()->getMvp(this->windowWidth, this->windowHeight);
		this->renderer->clear(0.2f, 0.3f, 0.3f, 1.0f);
		this->renderer->startDrawing(mvp);

		for (auto [entity, meshComp, textComp, transComp] : view.each()) {
			this->renderer->drawMesh(meshComp.mesh, textComp.texture, transComp.transform->getTransformMatrix());
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