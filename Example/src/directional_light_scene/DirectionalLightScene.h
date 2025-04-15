#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class DirectionalLightScene : public Scene {

public:

	~DirectionalLightScene();

private:

	std::unique_ptr<Camera3dController> cameraController;

	std::shared_ptr<Material> boxMaterial;
	std::shared_ptr<Mesh> boxMesh;

	std::shared_ptr<Material> lowPolyTree1Material;
	std::shared_ptr<Mesh> lowPolyTree1Mesh;

	std::shared_ptr<Material> lowPolyTree2Material;
	std::shared_ptr<Mesh> lowPolyTree2Mesh;

	std::shared_ptr<Material> lowPolyTree3Material;
	std::shared_ptr<Mesh> lowPolyTree3Mesh;

	std::shared_ptr<Material> lowPolyTree4Material;
	std::shared_ptr<Mesh> lowPolyTree4Mesh;

	std::shared_ptr<Material> terrainMaterial;
	std::shared_ptr<Mesh> cubeMesh;

	std::shared_ptr<CubeMap> skyboxCubeMap;

	void onStart() override;

	void onUpdate(float deltaTime) override;
};