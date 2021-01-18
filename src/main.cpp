#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer3D.h"
#include "Camera.h"
#include "CameraKeybordAndMouseInput.h"


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Universo Engine", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    Renderer3D render = Renderer3D();

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    CameraKeybordAndMouseInput cameraInput(&camera);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cameraInput.update(window, deltaTime);
        
        glm::mat4 model = glm::mat4(1.0f); 
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        render.setMatrixViewProjection(projection * view * model);

        render.start();
        render.drawCube(.5f, glm::vec3(3.0f, 3.0f, 0.0f));
        render.drawCube(.5f, glm::vec3(-3.0f, -3.0f, 0.0f));
        render.end();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



