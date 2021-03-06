#include "PLayerBehavior.h"


void PlayerBehavior::onStart() {
    camera =entity->getScene()->getCamera();
    this->movementSpeed = 6.0f;
    this->mouseSensitivity = 0.1f;
}

void PlayerBehavior::onUpdate(float deltaTime) {
    auto transformComponent = entity->getComponent<engine::TransformComponent>();
    this->camera->position = transformComponent.transform.position;
    this->camera->position.y += 2;

    this->processKeybordInput(deltaTime);
    this->processMouseInput();
}

void PlayerBehavior::processKeybordInput(float deltaTime) {
    auto rigidBodyComponent = entity->getComponent<engine::RigidBodyComponent>();

    glm::vec3 frontDirection = this->movementSpeed * camera->front;
    frontDirection.y = 0;
    glm::vec3 rightDirection = this->movementSpeed * camera->right;
    rightDirection.y = 0;

    if (engine::Input::keyPressed('W'))
        rigidBodyComponent.rigidBody->addForce(frontDirection * deltaTime);
    if (engine::Input::keyPressed('S'))
        rigidBodyComponent.rigidBody->addForce(-frontDirection * deltaTime);
    if (engine::Input::keyPressed('D'))
        rigidBodyComponent.rigidBody->addForce(rightDirection * deltaTime);
    if (engine::Input::keyPressed('A'))
        rigidBodyComponent.rigidBody->addForce(-rightDirection * deltaTime);


    glm::vec3 jumpForce = {0.0f, 15.0f, 0.0f};
    if (engine::Input::keyPressed('F')){
        rigidBodyComponent.rigidBody->addForce(jumpForce * deltaTime);
    }
}

void PlayerBehavior::processMouseInput() {
    double xpos, ypos;
    engine::Input::getCursorPos(&xpos, &ypos);

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


void PlayerBehavior::updateCameraAngles(float xoffset, float yoffset, GLboolean constrainPitch) {
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