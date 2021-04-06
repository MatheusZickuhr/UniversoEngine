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
		this->physicsWorld = physicsCommon.createPhysicsWorld();
		Input::init(this->window);
		
		this->currentScene = scene;
		this->initializeCurrentScene();

		this->setViewPortSize(this->windowWidth, this->windowHeight);
	}

	Engine::~Engine() {
		delete this->renderer;
		glfwTerminate();
	}

	void Engine::setViewPortSize(float newWindowWidth, float newWindowHeight) {
		this->windowWidth = newWindowWidth;
		this->windowHeight = newWindowHeight;
		this->renderer->setViewPortSize(newWindowWidth, newWindowHeight);
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
				this->physicsWorld->update(fixedDeltaTime);
				accumulator -= fixedDeltaTime;
			}

			// update the current scene with the updated physics world data
			float timeInterpolationFactor = accumulator / fixedDeltaTime;
			this->updateCurrentScenePhysics(timeInterpolationFactor);
			
			// update the current scene runtime
			this->currentScene->update(deltaTime);

			// in the end just render the current scene
			this->renderCurrentScene();
		
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
		ASSERT(registry.has<TransformComponent>(entity),
			"Entiy must have a transform component to have a rigid body component");
		ASSERT(registry.has<CollisionShapeComponent>(entity),
			"Entiy must have a collision shape component to have a rigid body component");

		auto& transform = registry.get<TransformComponent>(entity).transform;

		//create the RigidBody
		reactphysics3d::Vector3 position(transform.position.x, transform.position.y, transform.position.z);
		reactphysics3d::Vector3 rotation(transform.rotation.x, transform.rotation.y, transform.rotation.z);
		reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::fromEulerAngles(rotation);
		reactphysics3d::Transform rigidBodyTransform(position, orientation);
		reactphysics3d::RigidBody* rigidBody = this->physicsWorld->createRigidBody(rigidBodyTransform);

		// Create the collisionShape for the RigidBody
		reactphysics3d::ConvexPolyhedronShape* shape = nullptr;
		
		auto collisionShapeComp = registry.get<CollisionShapeComponent>(entity);

		switch (collisionShapeComp.collisionShape) {
			case CollisionShape::Box: {
				const reactphysics3d::Vector3 halfExtents(
					transform.scale.x, transform.scale.y, transform.scale.z);
				shape = physicsCommon.createBoxShape(halfExtents);
				break;
			}
			case CollisionShape::Capsule: {
				
				break;
			}
			case CollisionShape::Sphere: {

				break;
			}
		}

		ASSERT(shape != nullptr, "You must choose a valid/implemented collision shape");

		reactphysics3d::Transform collisionShapeTransform;
		rigidBody->addCollider(shape, collisionShapeTransform);

		// set the RigidBody type
		auto& rigidBodyComponent = registry.get<RigidBodyComponent>(entity);

		switch (rigidBodyComponent.type) {
			case RigidBodyType::Dynamic: {
				rigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
				break;
			}
			case RigidBodyType::Static: {
				rigidBody->setType(reactphysics3d::BodyType::STATIC);
				break; 
			}
			case RigidBodyType::Kinemattic: {
				rigidBody->setType(reactphysics3d::BodyType::KINEMATIC);
				break;
			}
		}
		
		// update the RigidBody component with the created rigid body
		rigidBodyComponent.rigidBody = RigidBody{ rigidBody };
	}

	void Engine::onRigidBodyComponentDestroyed(entt::registry& registry, entt::entity entity) {
		
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

	void Engine::renderCurrentScene() {
		auto view = this->currentScene->getRegistry()
		.view<MeshComponent, TextureComponent, TransformComponent>();

		auto mvp = currentScene->getCamera()->getMvp(this->windowWidth, this->windowHeight);
		this->renderer->clear(0.2f, 0.3f, 0.3f, 1.0f);
		this->renderer->startDrawing(mvp);

		for (auto [entity, meshComp, textComp, transComp] : view.each()) {
			this->renderer->drawMesh(meshComp.mesh, textComp.texture, transComp.transform.getTransformMatrix());
		}
		this->renderer->endDrawing();
	}

	void Engine::updateCurrentScenePhysics(float timeInterpolationFactor) {

		auto view = this->currentScene->getRegistry()
			.view<RigidBodyComponent, TransformComponent>();

		for (auto [entity, rbComp, transComp] : view.each()) {
			// perform transform interpolation
			auto& prevRbTransform = rbComp.rigidBody.prevTransform;
			auto& currentRbTransform = rbComp.rigidBody.rigidBodyPtr->getTransform();
			auto interpolatedTransform = reactphysics3d::Transform::interpolateTransforms(
				prevRbTransform, currentRbTransform, timeInterpolationFactor);
			rbComp.rigidBody.prevTransform = currentRbTransform;

			// update the transform component with the new values
			auto& rbPosition = interpolatedTransform.getPosition();
			transComp.transform.position = { rbPosition.x , rbPosition.y , rbPosition.z };

			auto& orientation = interpolatedTransform.getOrientation();
			transComp.transform.rotation = glm::eulerAngles(
				glm::quat(orientation.w, orientation.x, orientation.y, orientation.z)
			);
		}
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