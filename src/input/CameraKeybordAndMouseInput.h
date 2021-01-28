#pragma once

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class CameraKeybordAndMouseInput {

private:
	engine::Camera* camera;
    float movementSpeed;
    float mouseSensitivity;

    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

public:
	CameraKeybordAndMouseInput(engine::Camera* camera): movementSpeed(SPEED), mouseSensitivity(SENSITIVITY) {
		this->camera = camera;
	}

    void update(GLFWwindow* window, float deltaTime) {
        this->processKeybordInput(window, deltaTime);
        this->processMouseInput(window);
    }

private:

    void processKeybordInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            this->updateCameraPosition(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            this->updateCameraPosition(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            this->updateCameraPosition(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            this->updateCameraPosition(RIGHT, deltaTime);
    }

    void processMouseInput(GLFWwindow* window) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

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