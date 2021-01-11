#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Drawer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

class Renderer2D {

private:
	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Shader* vertexShader;
	Shader* fragShader;
	ShaderProgram* shaderProgram;
	Drawer* drawer;
    std::map<const char*, Texture*> textures;
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;

public:
    ~Renderer2D() {
        delete this->vertexArray;
        delete this->vertexBuffer;
        delete this->indexBuffer;
        delete this->vertexShader;
        delete this->fragShader;
        delete this->drawer;

        auto it = this->textures.end();
        while (it != this->textures.end()) {
            Texture* tex = it->second;
            delete tex;
        }

        this->textures.clear();
    }

	Renderer2D() {

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };


        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // vertex array object
        this->vertexArray = new VertexArray();

        // vertex buffer object
        this->vertexBuffer = new VertexBuffer(sizeof(vertices), vertices);
        this->vertexBuffer->addLayout(0, 3, 5 * sizeof(float), 0);
        // texture coords layout
        this->vertexBuffer->addLayout(1, 2, 5 * sizeof(float), 3 * sizeof(float));

        this->indexBuffer = new IndexBuffer(indices, 6);
 
        // shaders
        this->vertexShader = new Shader(VertexShader, "res/vert.glsl");
        this->fragShader = new Shader(FragmentShader, "res/frag.glsl");

        this->shaderProgram = new ShaderProgram();
        shaderProgram->attachShader(vertexShader->getId());
        shaderProgram->attachShader(fragShader->getId());
        shaderProgram->bind();

        shaderProgram->setUniform1i("TextureSlot", 0);

        this->drawer = new Drawer();

        this->proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        this->view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));
	}

    void drawQuad(float scale, glm::vec2 position, const char* textureName) {

        Texture* texture = this->textures[textureName];
        texture->bind();

        this->model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

        this->shaderProgram->setUniformMat4f("Model", this->model);
        this->shaderProgram->setUniformMat4f("View", this->view);
        this->shaderProgram->setUniformMat4f("Projection", this->proj);

        shaderProgram->setUniform1f("Scale", scale);
        //shaderProgram->setUniform4f("Color", color.x, color.y, color.z, color.w);

        this->drawer->drawWithIdexes(this->vertexArray, this->indexBuffer);
    }

    void createTexture(const char* filepath, const char* textureName) {
        this->textures[textureName] = new Texture(filepath);
    }
};