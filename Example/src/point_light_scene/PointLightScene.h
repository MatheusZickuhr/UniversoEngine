#pragma once

#include <UniversoEngine.h>
#include <memory>

using namespace engine;

class PointLightScene : public Scene {

public:

    ~PointLightScene();

private:

    std::unique_ptr<CameraController> cameraController;

    std::shared_ptr<Material> boxMaterial;
    std::shared_ptr<Mesh> boxMesh;

    std::shared_ptr<CubeMap> skyboxCubeMap;

    void onStart() override;

    void onUpdate(float deltaTime) override;
};