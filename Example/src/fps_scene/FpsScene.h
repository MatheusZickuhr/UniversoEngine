#include "scene/Scene.h"
#include "renderer/renderer_api/Texture.h"
#include "renderer/Mesh.h"
#include "scene/Components.h"
#include "PLayerBehavior.h"

class FpsScene : public engine::Scene {
private:
    engine::Texture* texture;
    engine::Mesh* mesh;

public:
    ~FpsScene();

    void onStart() override;

    void onUpdate(float deltaTime) override;

};