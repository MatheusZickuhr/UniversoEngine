#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class DirectionalLightScene : public Scene {

private:
	CameraController* cameraInput;

	std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("Example/resources/textures/crate/crate.jpg");
	std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("Example/resources/models/crate/crate.obj");

	std::shared_ptr<Material> lowPolyTree1Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree1/lowPolyTree1.png");
	std::shared_ptr<Mesh> lowPolyTree1Mesh = std::make_shared<Mesh >("Example/resources/models/lowPolyTree1/lowPolyTree1.obj");

	std::shared_ptr<Material> lowPolyTree2Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree2/lowPolyTree2.png");
	std::shared_ptr<Mesh> lowPolyTree2Mesh = std::make_shared<Mesh>("Example/resources/models/lowPolyTree2/lowPolyTree2.obj");

	std::shared_ptr<Material> lowPolyTree3Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree3/lowPolyTree3.png");
	std::shared_ptr<Mesh> lowPolyTree3Mesh = std::make_shared<Mesh>("Example/resources/models/lowPolyTree3/lowPolyTree3.obj");

	std::shared_ptr<Material> lowPolyTree4Material = std::make_shared<Material>("Example/resources/textures/lowPolyTree4/lowPolyTree4.png");
	std::shared_ptr<Mesh> lowPolyTree4Mesh = std::make_shared<Mesh>("Example/resources/models/lowPolyTree4/lowPolyTree4.obj");

	std::shared_ptr<Material> terrainMaterial = std::make_shared<Material>();
	std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("Example/resources/models/cube/cube.obj");

	std::shared_ptr<CubeMap> skyboxCubeMap;

public:

	~DirectionalLightScene();

private:

	void onStart() override;

	void onUpdate(float deltaTime) override;
};