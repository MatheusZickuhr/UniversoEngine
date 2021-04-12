#include "PLayerBehavior.h"


void PlayerBehavior::onStart() {
    camera = &entity->getScene()->getCamera();
    this->movementSpeed = 500.0f;
    this->mouseSensitivity = 0.1f;
}

void PlayerBehavior::onUpdate(float deltaTime) {
    auto& transformComponent = entity->getComponent<TransformComponent>();
    this->camera->position = transformComponent.transform.position;
    this->camera->position.y += 2;

    this->processKeybordInput(deltaTime);
    this->processMouseInput();
}

void PlayerBehavior::processKeybordInput(float deltaTime) {
    auto& rigidBodyComponent = entity->getComponent<RigidBodyComponent>();

    glm::vec3 frontDirection = this->movementSpeed * deltaTime * camera->front;
    frontDirection.y = 0;
    glm::vec3 rightDirection = this->movementSpeed * deltaTime * camera->right;
    rightDirection.y = 0;

    if (Input::keyPressed('W'))
        rigidBodyComponent.rigidBody->apllyForce(frontDirection);
    if (Input::keyPressed('S'))
        rigidBodyComponent.rigidBody->apllyForce(-frontDirection);
    if (Input::keyPressed('D'))
        rigidBodyComponent.rigidBody->apllyForce(rightDirection);
    if (Input::keyPressed('A'))
        rigidBodyComponent.rigidBody->apllyForce(-rightDirection);


    glm::vec3 jumpForce = {0.0f, 15.0f, 0.0f};
    if (Input::keyPressed('F')){
        rigidBodyComponent.rigidBody->apllyForce(jumpForce);
    }
}

void PlayerBehavior::processMouseInput() {
    double xpos, ypos;
    Input::getCursorPos(&xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    this->updateCameraAngles(xoffset, yoffset);
}


void PlayerBehavior::updateCameraAngles(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->camera->yaw += xoffset;
    this->camera->pitch += yoffset;

    if (constrainPitch) {
        if (this->camera->pitch > 89.0f)
            this->camera->pitch = 89.0f;
        if (this->camera->pitch < -89.0f)
            this->camera->pitch = -89.0f;
    }

    this->camera->updateVectors();
}