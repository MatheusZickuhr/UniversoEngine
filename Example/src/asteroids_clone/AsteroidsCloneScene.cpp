#include "AsteroidsCloneScene.h"
#include "ShipBehavior.h"
#include "AsteroidBehavior.h"
#include "ProjectileBehavior.h"

AsteroidsCloneScene::~AsteroidsCloneScene() {

}

void AsteroidsCloneScene::onStart() {
	this->camera.position = { 0.0f, 0.0f, 20.0f };


	coneMaterial = std::make_shared<Material>();
	cubeMaterial = std::make_shared<Material>();
	coneMesh = std::make_shared<Mesh>("Example/resources/models/cone/cone.obj");
	cubeMesh = std::make_shared<Mesh>("Example/resources/models/cube/cube.obj");

	skyboxCubeMap = CubeMap::createCubeMapFromFile(
		{
			"Example/resources/textures/exampleSkyBox/right.jpg",
			"Example/resources/textures/exampleSkyBox/left.jpg",
			"Example/resources/textures/exampleSkyBox/top.jpg",
			"Example/resources/textures/exampleSkyBox/bottom.jpg",
			"Example/resources/textures/exampleSkyBox/front.jpg",
			"Example/resources/textures/exampleSkyBox/back.jpg"
		}
	);

	this->setSkyBoxCubeMap(this->skyboxCubeMap);


	 //create player entity
	{
		playerOptional = { createEntity() };
		auto& player = playerOptional.value();
		player.addComponent<MaterialComponent>(coneMaterial);
		player.addComponent<TransformComponent>();
		player.addComponent<DynamicMeshComponent>(coneMesh);
		player.addComponent<BehaviorComponent>().bindBehavior<ShipBehavior>();
		player.addComponent<CollisionShapeComponent>(CollisionShape::Box);
		player.addComponent<CollisionBodyComponent>();
		player.addComponent<NameComponent>("player");
	}

	// sun light
	auto directionalLight = createEntity();
	directionalLight.addComponent<DirectionalLightComponent>();
	directionalLight.addComponent<TransformComponent>();
	auto& transComp = directionalLight.getComponent<TransformComponent>();
	transComp.transform.position = { -1.0f, 4.0f, -1.0f };
	
}

void AsteroidsCloneScene::onUpdate(float deltaTime) {

	timeElapsedFromLastAsteroidCreated += deltaTime;

	if (timeElapsedFromLastAsteroidCreated > timeToSpawnNewSteriod) {
		timeElapsedFromLastAsteroidCreated = 0.0f;
		timeToSpawnNewSteriod = random.random() * 3.0f;
		createAsteroidEntity();
	}

	handlePlayerShooting(deltaTime);
}

void AsteroidsCloneScene::handlePlayerShooting(float deltaTime) {
	if (Input::keyPressed(Input::KEY_SPACE) && timeElapsedFromLastShot > timeBetweenShots) {
		timeElapsedFromLastShot = 0.0f;
		createProjectileEntity();
	}

	timeElapsedFromLastShot += deltaTime;
}

void AsteroidsCloneScene::createProjectileEntity() {

	if (!playerOptional.has_value() || !playerOptional.value().isValid()) {
		return;
	}
	auto& player = playerOptional.value();

	auto& playerTransformComponent = player.getComponent<TransformComponent>();

	Entity projectile = createEntity();

	projectile.addComponent<MaterialComponent>(cubeMaterial);
	projectile.addComponent<TransformComponent>();

	auto& projectileTransformComponent = projectile.getComponent<TransformComponent>();
	projectileTransformComponent.transform.position = playerTransformComponent.transform.position;
	projectileTransformComponent.transform.scale *= 0.2f;

	projectile.addComponent<CollisionShapeComponent>(CollisionShape::Box);
	projectile.addComponent<CollisionBodyComponent>();

	projectile.addComponent<DynamicMeshComponent>(cubeMesh);
	projectile.addComponent<BehaviorComponent>().bindBehavior<ProjectileBehavior>();

	float zRotation = playerTransformComponent.transform.rotation.z + (Math::PI / 2.0f);

	glm::vec2 projectileDirection = glm::normalize(glm::vec2(glm::cos(zRotation), glm::sin(zRotation)));

	projectile.getComponent<BehaviorComponent>()
		.getBehaviorInstance<ProjectileBehavior>()
		->setDirection(projectileDirection);

}

void AsteroidsCloneScene::createAsteroidEntity() {
	LOG("Asteroiod spawned");

	auto asteroid = createEntity();
	asteroid.addComponent<MaterialComponent>(cubeMaterial);

	float yPos = random.random() > 0.5f ? -10.0f : 10.0f;
	float xPos = Math::map(random.random() * 10.f, 0.0f, 10.0f, -10.0f, 10.0f);

	asteroid.addComponent<TransformComponent>().transform.position = { xPos, yPos, 0.0f };
	asteroid.addComponent<DynamicMeshComponent>(cubeMesh);
	asteroid.addComponent<NameComponent>("asteroid");
	asteroid.addComponent<CollisionShapeComponent>(CollisionShape::Box);
	asteroid.addComponent<CollisionBodyComponent>();
	asteroid.addComponent<BehaviorComponent>().bindBehavior<AsteroidBehavior>();

	glm::vec2 direction = { random.random() * 2.0f, -yPos };

	asteroid.getComponent<BehaviorComponent>()
		.getBehaviorInstance<AsteroidBehavior>()
		->setDirection(glm::normalize(direction));
}
