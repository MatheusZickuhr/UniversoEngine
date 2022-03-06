#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class PointLightScene : public Scene {

private:
    CameraController* cameraInput;

    Material boxMaterial{ "Example/resources/textures/crate/crate.jpg" };
    Mesh boxMesh{ "Example/resources/models/crate/crate.obj" };

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

    ~PointLightScene();

private:

    void onStart() override;

    void onUpdate(float deltaTime) override;
};