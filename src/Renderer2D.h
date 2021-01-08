#include "Drawer.h"
#include "engine.h"


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

        shaderProgram->setUniform4f("Color", 0.0f, 1.0f, 0.0f, 1.0f);

        glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));

        // model view projection matrix
        glm::mat4 mvp = proj * view * model;

        shaderProgram->setUniformMat4f("Mvp", mvp);

        shaderProgram->setUniform1f("Scale", 0.3);

        this->drawer = new Drawer();
	}

	void drawQuad() {
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        this->shaderProgram->setUniform4f("Color", 0.0f, greenValue, 0.0f, 1.0f);

        this->drawer->drawWithIdexes(this->vertexArray, this->indexBuffer);
	}
};