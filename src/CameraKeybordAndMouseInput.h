const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class CameraKeybordAndMouseInput {

private:
	Camera* camera;
    float movementSpeed;
    float mouseSensitivity;

    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

public:
	CameraKeybordAndMouseInput(Camera* camera): movementSpeed(SPEED), mouseSensitivity(SENSITIVITY) {
		this->camera = camera;
	}

    void update(GLFWwindow* window, float deltaTime) {
        this->processKeybordInput(window, deltaTime);
        this->processMouseInput(window);
    }

private:

    void processKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = this->movementSpeed * deltaTime;
        if (direction == FORWARD)
            this->camera->setPosition(this->camera->getPosition() + (this->camera->getFront() * velocity));
            //this->position += this->front * velocity;
        if (direction == BACKWARD)
            this->camera->setPosition(this->camera->getPosition() - (this->camera->getFront() * velocity));
           // this->position -= this->front * velocity;
        if (direction == LEFT)
            this->camera->setPosition(this->camera->getPosition() - (this->camera->getRight() * velocity));
           // this->position -= this->right * velocity;
        if (direction == RIGHT)
            this->camera->setPosition(this->camera->getPosition() + (this->camera->getRight() * velocity));
            //this->position += this->right * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->mouseSensitivity;
        yoffset *= this->mouseSensitivity;

        this->camera->setYaw(this->camera->getYaw() + xoffset);
        this->camera->setPitch(this->camera->getPitch() + yoffset);
        

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->camera->getPitch() > 89.0f)
                this->camera->setPitch(89.0f);
            if (this->camera->getPitch() < -89.0f)
                this->camera->setPitch(-89.0f);
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        this->camera->updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
        this->camera->setZoom(this->camera->getZoom() - (float)yoffset);
        
        if (this->camera->getZoom() < 1.0f)
            this->camera->setZoom(1.0f);
        if (this->camera->getZoom() > 45.0f)
            this->camera->setZoom(45.0f);
    }

    void processKeybordInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            this->processKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            this->processKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            this->processKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            this->processKeyboard(RIGHT, deltaTime);
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

        this->processMouseMovement(xoffset, yoffset);
    }



};