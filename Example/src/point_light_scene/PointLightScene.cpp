#include "PointLightScene.h"

PointLightScene::~PointLightScene() {
}

void PointLightScene::onStart() {

    this->camera.position = { 0.0f, 0.0f, 5.0f };

    cameraController = std::make_unique<CameraController>(this->camera);

    boxMaterial = std::make_shared<Material>("Example/resources/textures/crate/crate.jpg");
    boxMesh = std::make_shared<Mesh>("Example/resources/models/crate/crate.obj");

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

    {
        auto box = createEntity();
        box.addComponent<MaterialComponent>(boxMaterial);
        box.addComponent<TransformComponent>();

        auto& boxTransformComponent = box.getComponent<TransformComponent>();
        boxTransformComponent.transform.position = { 4.0f, 0.0f, 0.0f };

        box.addComponent<StaticMeshComponent>(boxMesh);
    }

    {
        auto box = createEntity();
        box.addComponent<MaterialComponent>(boxMaterial);
        box.addComponent<TransformComponent>();

        auto& boxTransformComponent = box.getComponent<TransformComponent>();
        boxTransformComponent.transform.position = { -4.0f, 0.0f, 0.0f  };

        box.addComponent<StaticMeshComponent>(boxMesh);
    }

    {
        auto box = createEntity();
        box.addComponent<MaterialComponent>(boxMaterial);
        box.addComponent<TransformComponent>();

        auto& boxTransformComponent = box.getComponent<TransformComponent>();
        boxTransformComponent.transform.position = { 0.0f, 0.0f, -4.0f };

        box.addComponent<StaticMeshComponent>(boxMesh);
    }

    // create a floor entity
    {
        auto floor = createEntity();
        floor.addComponent<MaterialComponent>(boxMaterial);
        floor.addComponent<TransformComponent>();

        auto& floorTransformComponent = floor.getComponent<TransformComponent>();
        floorTransformComponent.transform.position.y = -2.0f;
        floorTransformComponent.transform.scale = { 10.0f, 0.5f, 10.0f };

        floor.addComponent<StaticMeshComponent>(boxMesh);

        floor.addComponent<CollisionShapeComponent>(CollisionShape::Box);
        floor.addComponent<RigidBodyComponent>(RigidBodyType::Static);
    }

    {
    	auto pointLight = createEntity();
    	pointLight.addComponent<PointLightComponent>();
    	pointLight.addComponent<TransformComponent>();
    	auto& transComp = pointLight.getComponent<TransformComponent>();
    	transComp.transform.position = { 0.0f, 2.0f, 0.0f };
    }

}

void PointLightScene::onUpdate(float deltaTime) {
    cameraController->update(deltaTime);
}