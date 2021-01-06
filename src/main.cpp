#include <stdio.h>
#include <iostream>
#include <string>

#include "engine.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include "Shader.h"
#include "ShaderProgram.h"

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Universo Engine", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // vertex array object
    VertexArray();

    // vertex buffer object
    VertexBuffer(sizeof(vertices), vertices);

    // buffer layout
    BufferLayout();
    
    // shaders
    auto vertexShader = Shader(VertexShader, "res/vert.glsl");
    auto fragShader = Shader(FragmentShader, "res/frag.glsl");

    auto shaderProgram = ShaderProgram();
    shaderProgram.attachShader(vertexShader.getId());
    shaderProgram.attachShader(fragShader.getId());
    shaderProgram.bind();

    shaderProgram.setUniform4f("Color", 0.0f, 1.0f, 0.0f, 1.0f);
    
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shaderProgram.setUniform4f("Color", 0.0f, greenValue, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}