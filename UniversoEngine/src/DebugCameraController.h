#pragma once

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
    DebugCameraController(std::shared_ptr<engine::Camera> camera): movementSpeed(SPEED), mouseSensitivity(SENSITIVITY) {
		this->camera = camera;
	}

    void update(float deltaTime) {
        this->processKeybordInput(deltaTime);
        this->processMouseInput();
    }

private:

    void processKeybordInput(float deltaTime) {
        if (engine::Input::keyPressed('W'))
            this->updateCameraPosition(FORWARD, deltaTime);
        if (engine::Input::keyPressed('S'))
            this->updateCameraPosition(BACKWARD, deltaTime);
        if (engine::Input::keyPressed('A'))
            this->updateCameraPosition(LEFT, deltaTime);
        if (engine::Input::keyPressed('D'))
            this->updateCameraPosition(RIGHT, deltaTime);
    }

    void processMouseInput() {
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

    void updateCameraPosition(CameraMovement direction, float deltaTime) {
        float velocity = this->movementSpeed * deltaTime;
        if (direction == FORWARD)
            this->camera->position += this->camera->front * velocity;
            
        if (direction == BACKWARD)
            this->camera->position -= this->camera->front * velocity;
         
        if (direction == LEFT)
            this->camera->position -= this->camera->right * velocity;
           
        if (direction == RIGHT)
            this->camera->position += this->camera->right * velocity;
            
    }

    void updateCameraAngles(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= this->mouseSensitivity;
        yoffset *= this->mouseSensitivity;

        this->camera->yaw += xoffset;
        this->camera->pitch += yoffset;
        
        if (constrainPitch) {
            if (this->camera->pitch > 89.0f)
                this->camera->pitch = 89.0f;
            if (this->camera->pitch  < -89.0f)
                this->camera->pitch = -89.0f;
        }

        this->camera->updateVectors();
    }
};