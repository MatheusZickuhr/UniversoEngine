#pragma once

#include <memory>
#include "../renderer/Camera.h"
#include "../input/Input.h"

namespace engine {

    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;

    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class DebugCameraController {

    private:
        std::shared_ptr<engine::Camera> camera;
        float movementSpeed;
        float mouseSensitivity;

        float lastX = 0;
        float lastY = 0;
        bool firstMouse = true;

    public:
        DebugCameraController(std::shared_ptr<engine::Camera> camera) ;

        void update(float deltaTime);

    private:

        void processKeybordInput(float deltaTime);

        void processMouseInput();

        void updateCameraPosition(CameraMovement direction, float deltaTime);

        void updateCameraAngles(float xoffset, float yoffset, GLboolean constrainPitch = true);
    };
} 