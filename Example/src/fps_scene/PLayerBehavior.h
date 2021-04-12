#include <memory>
#include "scene/Behavior.h"
#include "input/Input.h"
#include "scene/Components.h"

class PlayerBehavior : public engine::Behavior {

public:

    using engine::Behavior::Behavior;

    void onStart() override;

    void onUpdate(float deltaTime) override;

private:

    engine::Camera* camera;
    float movementSpeed;
    float mouseSensitivity;
    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

    void processKeybordInput(float deltaTime);

    void processMouseInput();

    void updateCameraAngles(float xoffset, float yoffset, bool constrainPitch = true);
};