#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer3D.h"


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Universo Engine", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    Renderer3D render = Renderer3D();

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        render.start();
        render.drawCube(.5f, glm::vec3(3.0f, 3.0f, 0.0f));
        render.drawCube(.5f, glm::vec3(-3.0f, -3.0f, 0.0f));
        render.end();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}