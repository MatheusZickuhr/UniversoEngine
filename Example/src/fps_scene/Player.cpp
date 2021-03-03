#include "Player.h"

void Player::onStart() {
    this->movementSpeed = 6.0f;
    this->mouseSensitivity = 0.1f;
}

void Player::setCamera(std::shared_ptr<engine::Camera> camera) {
    this->camera = camera;
}

void Player::onUpdate(float deltaTime) {
    this->camera->position = this->transform->position;
    this->camera->position.y += 2;

    this->processKeybordInput(deltaTime);
    this->processMouseInput();
}

void Player::processKeybordInput(float deltaTime) {
    glm::vec3 frontDirection = this->movementSpeed * camera->front;
    frontDirection.y = 0;
    glm::vec3 rightDirection = this->movementSpeed * camera->right;
    rightDirection.y = 0;

    if (engine::Input::keyPressed('W'))
        this->rigidBody->addForce(frontDirection * deltaTime);
    if (engine::Input::keyPressed('S'))
        this->rigidBody->addForce(-frontDirection * deltaTime);
    if (engine::Input::keyPressed('D'))
        this->rigidBody->addForce(rightDirection * deltaTime);
    if (engine::Input::keyPressed('A'))
        this->rigidBody->addForce(-rightDirection * deltaTime);


    glm::vec3 jumpForce = {0.0f, 15.0f, 0.0f};
    if (engine::Input::keyPressed('F')){
        this->rigidBody->addForce(jumpForce * deltaTime);
    }
}

void Player::processMouseInput() {
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


void Player::updateCameraAngles(float xoffset, float yoffset, GLboolean constrainPitch) {
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
