#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer2D.h"


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

    Renderer2D renderer2d = Renderer2D();

    renderer2d.createTexture("res/textures/eye.png", "eye");
    renderer2d.createTexture("res/textures/star.png", "star");
    renderer2d.createTexture("res/textures/ray.png", "ray");

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        renderer2d.start();

        renderer2d.drawQuad(.3f, glm::vec2(1.0f, 0.3f), "eye");
        renderer2d.drawQuad(.3f, glm::vec2(1.0f, -0.5f), "star");
        renderer2d.drawQuad(.5f, glm::vec2(-1.0f, 0.3f), "ray");

        renderer2d.end();

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}