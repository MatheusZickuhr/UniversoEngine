#include <UniversoEngine.h>

using namespace engine;

class PlayerBehavior : public Behavior {

public:

    using Behavior::Behavior;

    void onStart() override;

    void onUpdate(float deltaTime) override;

private:

    Camera* camera;
    float movementSpeed;
    float mouseSensitivity;
    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

    void processKeybordInput(float deltaTime);

    void processMouseInput();

    void updateCameraAngles(float xoffset, float yoffset, bool constrainPitch = true);
};