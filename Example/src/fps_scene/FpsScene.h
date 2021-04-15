#include <UniversoEngine.h>

#include "PLayerBehavior.h"

using namespace engine;

class FpsScene : public Scene {
private:
    Material* boxMaterial;
    Mesh* boxMesh;

    Material* grassCubeMaterial;
    Mesh* grassCubeMesh;

public:
    ~FpsScene();

private:

    void onStart() override;

    void onUpdate(float deltaTime) override;
};