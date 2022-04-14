#include "ProceduralTerrainScene.h"

const float chunkSize = 50.0f;

const float worldMaxSize = 10000.0f;
const float worldMaxXzise = worldMaxSize;
const float worldMinXzise = -worldMaxSize;
const float worldMaxZsize = worldMaxSize;
const float worldMinZsize = -worldMaxSize;

const float drawDistance = 50.0f;


ProceduralTerrainScene::~ProceduralTerrainScene() {
  
}

void ProceduralTerrainScene::onStart() {

    this->setSkyBoxCubeMap(this->skyboxCubeMap);

    this->camera.position = { 100.0f, 5.0f, 100.0f };


    cameraInput = std::make_unique<CameraController>(this->camera);

    Math::reseedPerlinNoise(0);


    for (float x = worldMinXzise; x < worldMaxXzise; x += chunkSize) {
        for (float z = worldMinZsize; z < worldMaxZsize; z += chunkSize) {
            Chunk chunk;
            chunk.minX = x;
            chunk.maxX = x + chunkSize;
            chunk.minZ = z;
            chunk.maxZ = z + chunkSize;
            chunks.push_back(chunk);
        }
    }

    // sun light
    {
        auto directionalLight = createEntity();
        directionalLight.addComponent<DirectionalLightComponent>();
        directionalLight.addComponent<TransformComponent>();
        auto& transComp = directionalLight.getComponent<TransformComponent>();
        transComp.transform.position = { -1.0f, 4.0f, -1.0f };
    }
}

void ProceduralTerrainScene::onUpdate(float deltaTime) {
    cameraInput->update(deltaTime);

    for (Chunk& chunk : chunks) {
        float distance = glm::distance(
            glm::vec2(camera.position.x, camera.position.z),
            glm::vec2(
                (chunk.minX + chunk.maxX) / 2,
                (chunk.minZ + chunk.maxZ) / 2
            )
        );

        if (glm::abs(distance) > drawDistance) {
            despawnChunk(chunk);
        }
        else {
            spawnChunk(chunk);
        }
    }

}

void ProceduralTerrainScene::spawnChunk(Chunk& chunk) {
    if (chunk.spawned) {
        //LOG("chunk spawned already");
        return;
    }
    
    chunk.spawned = true;

    for (float x = chunk.minX; x < chunk.maxX; x += 2.0f) {
        for (float z = chunk.minZ; z < chunk.maxZ; z += 2.0f) {

            // create the box entity
            auto box = createEntity();
            box.addComponent<MaterialComponent>(&boxMaterial);
            box.addComponent<TransformComponent>();
            auto& boxTransformComponent = box.getComponent<TransformComponent>();
            float y = Math::map(Math::perlinNoise2D(x / 200.0f, z / 200.0f), 0.0f, 1.0f, -15.0f, 15.0f);
            boxTransformComponent.transform.position = { x, round(y) * 2.0f, z };
            box.addComponent<StaticMeshComponent>(&boxMesh);

            // add to the chunk
            chunk.entities.push_back(box);
        }
    }
}

void ProceduralTerrainScene::despawnChunk(Chunk& chunk) {
    for (Entity& entity : chunk.entities) {
        destroyEntity(entity);
    }
    chunk.entities.clear();

    chunk.spawned = false;
}
