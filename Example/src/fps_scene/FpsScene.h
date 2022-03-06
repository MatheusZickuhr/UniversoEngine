#include <UniversoEngine.h>

#include "PLayerBehavior.h"
#include <memory>

using namespace engine;

class FpsScene : public Scene {

private:
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
    ~FpsScene();

private:

    void onStart() override;

    void onUpdate(float deltaTime) override;
};