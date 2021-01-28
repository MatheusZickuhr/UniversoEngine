#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer/Renderer3D.h"
#include "renderer/Camera.h"
#include "input/CameraKeybordAndMouseInput.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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
    
    engine::Mesh crateMesh("res/models/crate/crate.obj");
    engine::Texture crateTexture1("res/textures/crate/crate.jpg");
    engine::Texture crateTexture2("res/textures/crate/crate.png");
    engine::Texture crateTexture3("res/textures/crate/crate2.png");

    engine::Renderer3D renderer;

    engine::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    CameraKeybordAndMouseInput cameraInput(&camera);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cameraInput.update(window, deltaTime);
        
        renderer.clear(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.setModelViewProjectionMatrix(camera.getModelViewProjectionMatrix(WIDTH, HEIGHT));
        renderer.startDrawing();
        renderer.drawMesh(&crateMesh, &crateTexture1, .5f, glm::vec3(3.0f, 3.0f, 0.0f));
        renderer.drawMesh(&crateMesh, &crateTexture2, .5f, glm::vec3(-3.0f, -3.0f, 0.0f));
        renderer.drawMesh(&crateMesh, &crateTexture3, .5f, glm::vec3(-3.0f, -3.0f, -3.0f));
        renderer.endDrawing();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



