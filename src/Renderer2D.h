#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Drawer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Renderer2D {

private:
	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Shader* vertexShader;
	Shader* fragShader;
	ShaderProgram* shaderProgram;
	Drawer* drawer;

public:
    ~Renderer2D() {
        delete this->vertexArray;
        delete this->vertexBuffer;
        delete this->indexBuffer;
        delete this->vertexShader;
        delete this->fragShader;
        delete this->drawer;
    }

	Renderer2D() {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        // vertex array object
        this->vertexArray = new VertexArray();

        // vertex buffer object
        this->vertexBuffer = new VertexBuffer(sizeof(vertices), vertices);
        this->vertexBuffer->addLayout(0, 3, 3 * sizeof(float), 0);

        this->indexBuffer = new IndexBuffer(indices, 6);

        // shaders
        this->vertexShader = new Shader(VertexShader, "res/vert.glsl");
        this->fragShader = new Shader(FragmentShader, "res/frag.glsl");

        this->shaderProgram = new ShaderProgram();
        shaderProgram->attachShader(vertexShader->getId());
        shaderProgram->attachShader(fragShader->getId());
        shaderProgram->bind();

        this->drawer = new Drawer();
	}

	void drawQuad(float scale, glm::vec2 position, glm::vec4 color) {

        glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        glm::mat4 mvp = proj * view * model;
   
        this->shaderProgram->setUniformMat4f("Mvp", mvp);
        shaderProgram->setUniform1f("Scale", scale);
        shaderProgram->setUniform4f("Color", color.x, color.y, color.z, color.w);

        this->drawer->drawWithIdexes(this->vertexArray, this->indexBuffer);
	}
};