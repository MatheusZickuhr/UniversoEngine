#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class AsteroidsCloneScene : public Scene {

public:

	~AsteroidsCloneScene();

private:

	std::shared_ptr<CubeMap> skyboxCubeMap;
	std::shared_ptr<Material> coneMaterial;
	std::shared_ptr<Material> cubeMaterial;
	std::shared_ptr<Mesh> coneMesh;
	std::shared_ptr<Mesh> cubeMesh;

	Random random{};

	std::optional<Entity> playerOptional;

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