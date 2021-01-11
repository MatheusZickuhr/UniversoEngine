#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer2D.h"


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    renderer2d.createTexture("res/textures/ray.png", "ray");

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);

        renderer2d.drawQuad(
            .3f,
            glm::vec2(0.0f, 0),
            "eye"
        );

        renderer2d.drawQuad(
            .3f,
            glm::vec2(0.5f, 0),
            "ray"
        );


        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}