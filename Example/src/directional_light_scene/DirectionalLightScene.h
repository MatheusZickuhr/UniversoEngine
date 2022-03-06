#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class DirectionalLightScene : public Scene {

private:
	CameraController* cameraInput;

	Material boxMaterial{ "Example/resources/textures/crate/crate.jpg" };
	Mesh boxMesh{ "Example/resources/models/crate/crate.obj" };

	Material lowPolyTree1Material { "Example/resources/textures/lowPolyTree1/lowPolyTree1.png" };
	Mesh lowPolyTree1Mesh { "Example/resources/models/lowPolyTree1/lowPolyTree1.obj" };

	Material lowPolyTree2Material{ "Example/resources/textures/lowPolyTree2/lowPolyTree2.png" };
	Mesh lowPolyTree2Mesh{ "Example/resources/models/lowPolyTree2/lowPolyTree2.obj" };

	Material lowPolyTree3Material{ "Example/resources/textures/lowPolyTree3/lowPolyTree3.png" };
	Mesh lowPolyTree3Mesh{ "Example/resources/models/lowPolyTree3/lowPolyTree3.obj" };

	Material lowPolyTree4Material{ "Example/resources/textures/lowPolyTree4/lowPolyTree4.png" };
	Mesh lowPolyTree4Mesh{ "Example/resources/models/lowPolyTree4/lowPolyTree4.obj" };

	Material terrainMaterial { };
	Mesh cubeMesh { "Example/resources/models/cube/cube.obj" };

	std::shared_ptr<CubeMap> skyboxCubeMap = CubeMap::createCubeMapFromFile(
		{
			"Example/resources/textures/exampleSkyBox/right.jpg",
			"Example/resources/textures/exampleSkyBox/left.jpg",
			"Example/resources/textures/exampleSkyBox/top.jpg",
			"Example/resources/textures/exampleSkyBox/bottom.jpg",
			"Example/resources/textures/exampleSkyBox/front.jpg",
			"Example/resources/textures/exampleSkyBox/back.jpg"
		}
	);

public:

	~DirectionalLightScene();

private:

	void onStart() override;

	void onUpdate(float deltaTime) override;
};