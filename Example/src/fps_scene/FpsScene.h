#include "scene/Scene.h"
#include "utils/DebugCameraController.h"
#include "renderer/Mesh.h"
#include "renderer/renderer_api/Texture.h"

#include "Floor.h"
#include "Player.h"
#include "Obstacle.h"


class FpsScene : public engine::Scene {

    private:
     std::shared_ptr<engine::DebugCameraController> cameraInput;

    public:
        void onStart() override;
        void onUpdate(float deltaTime) override;
};