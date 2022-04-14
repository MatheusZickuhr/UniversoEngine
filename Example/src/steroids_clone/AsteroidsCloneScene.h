#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class AsteroidsCloneScene : public Scene {

private:
	CameraController* cameraInput;

	Material coneMaterial {};
	Mesh coneMesh { "Example/resources/models/cone/cone.obj" };

	std::shared_ptr<CubeMap> skyboxCubeMap;

	Material cubeMaterial{};
	Mesh cubeMesh{ "Example/resources/models/cube/cube.obj" };

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