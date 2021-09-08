#include "ProceduralTerrainScene.h"

ProceduralTerrainScene::~ProceduralTerrainScene() {

    delete cameraInput;
}

void ProceduralTerrainScene::onStart() {

    this->setSkyBoxCubeMap(&this->skyboxCubeMap);

    this->camera.position = { 100.0f, 5.0f, 100.0f };

    cameraInput = new CameraController(this->camera);

    Math::reseedPerlinNoise(0);

    for (float x = 0.0f; x < 200.0f; x += 2.0f) {
        for (float z = 0.0f; z < 200.0f; z += 2.0f) {

            auto box = createEntity();
            box->addComponent<MaterialComponent>(&boxMaterial);
            box->addComponent<TransformComponent>();

            auto& boxTransformComponent = box->getComponent<TransformComponent>();

            float y = Math::map(Math::perlinNoise2D(x / 200.0f, z / 200.0f), 0.0f, 1.0f, -15.0f, 15.0f);

            boxTransformComponent.transform.position = { x, round(y) * 2.0f, z };

            box->addComponent<StaticMeshComponent>(&boxMesh);
        }
    }

    // sun light
    {
        auto directionalLight = createEntity();
        directionalLight->addComponent<DirectionalLightComponent>();
        directionalLight->addComponent<TransformComponent>();
        auto& transComp = directionalLight->getComponent<TransformComponent>();
        transComp.transform.position = { -1.0f, 4.0f, -1.0f };
    }
}

void ProceduralTerrainScene::onUpdate(float deltaTime) {
    cameraInput->update(deltaTime);
}