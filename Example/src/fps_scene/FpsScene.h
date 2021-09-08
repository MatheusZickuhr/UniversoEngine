#include <UniversoEngine.h>

#include "PLayerBehavior.h"

using namespace engine;

class FpsScene : public Scene {

private:
    Material boxMaterial{ "Example/resources/textures/crate/crate.jpg" };
    Mesh boxMesh{ "Example/resources/models/crate/crate.obj" };

    CubeMap skyboxCubeMap {
            {
                    "Example/resources/textures/exampleSkyBox/right.jpg",
                    "Example/resources/textures/exampleSkyBox/left.jpg",
                    "Example/resources/textures/exampleSkyBox/top.jpg",
                    "Example/resources/textures/exampleSkyBox/bottom.jpg",
                    "Example/resources/textures/exampleSkyBox/front.jpg",
                    "Example/resources/textures/exampleSkyBox/back.jpg"
            }
    };

public:
    ~FpsScene();

private:

    void onStart() override;

    void onUpdate(float deltaTime) override;
};