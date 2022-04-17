#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class AsteroidsCloneScene : public Scene {

private:

	std::shared_ptr<Material> coneMaterial = std::make_shared<Material>();
	std::shared_ptr<Mesh> coneMesh = std::make_shared<Mesh>("Example/resources/models/cone/cone.obj");

	std::shared_ptr<CubeMap> skyboxCubeMap;

	std::shared_ptr<Material> cubeMaterial = std::make_shared<Material>();
	std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("Example/resources/models/cube/cube.obj");

	Random random{};

	std::optional<Entity> playerOptional;

public:

	~AsteroidsCloneScene();

private:

	float timeElapsedFromLastAsteroidCreated = 0.0f;

	float timeToSpawnNewSteriod = 0.0f;

	float timeElapsedFromLastShot = 0.0f;
	const float timeBetweenShots = 0.1f;

	void onStart() override;

	void onUpdate(float deltaTime) override;

	void createAsteroidEntity();

	void handlePlayerShooting(float deltaTime);

	void createProjectileEntity();
};