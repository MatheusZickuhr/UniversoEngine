#pragma once
#include <memory>
#include "scene/GameObject.h"
#include "renderer/Camera.h"
#include "input/Input.h"


class Player: public engine::GameObject {

private:
    std::shared_ptr<engine::Camera> camera;
    float movementSpeed;
    float mouseSensitivity;
    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

public:
    using engine::GameObject::GameObject;

    void onUpdate(float deltaTime) override;
    void onStart() override;
    void setCamera(std::shared_ptr<engine::Camera> camera);

private:

    void processKeybordInput(float deltaTime);

    void processMouseInput();

    void updateCameraAngles(float xoffset, float yoffset, GLboolean constrainPitch = true);
};
