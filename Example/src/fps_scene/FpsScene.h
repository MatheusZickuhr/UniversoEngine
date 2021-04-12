#include <UniversoEngine.h>

#include "PLayerBehavior.h"

using namespace engine;

class FpsScene : public Scene {
private:
    Texture* boxTexture;
    Mesh* boxMesh;

    Texture* grassCubeTexture;
    Mesh* grassCubeMesh;

public:
    ~FpsScene();

private:

    void onStart() override;

    void onUpdate(float deltaTime) override;
};