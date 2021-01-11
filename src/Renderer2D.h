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
#include <array>


const unsigned int maxQuads = 100;
const unsigned int maxVertices = maxQuads * 4;
const unsigned int maxIndices = maxQuads * 6;


struct Vertex {
    glm::vec3 position;
    glm::vec2 textureCoords;
};

class Renderer2D {

private:
	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Shader* vertexShader;
	Shader* fragShader;
	ShaderProgram* shaderProgram;
	Drawer* drawer;
    Vertex* renderDataBufferStart;
    Vertex* renderDataBuffer;
    unsigned int vertexCount;
    unsigned int IndexCount;
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

        delete[] this->renderDataBufferStart;
    }

	Renderer2D() {

        this->renderDataBufferStart = new Vertex[maxVertices];
        this->renderDataBuffer = this->renderDataBufferStart;

        // vertex array object
        this->vertexArray = new VertexArray();

        // vertex buffer object
        this->vertexBuffer = new VertexBuffer(sizeof(Vertex) * maxVertices);
        this->vertexBuffer->addLayout(0, 3, sizeof(Vertex), offsetof(Vertex, position));
        this->vertexBuffer->addLayout(1, 2, sizeof(Vertex), offsetof(Vertex, textureCoords));

        unsigned int indices[maxIndices];
        unsigned int offset = 0;
        for (int i = 0; i < maxIndices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        this->indexBuffer = new IndexBuffer(indices, maxIndices);
 
        // shaders
        this->vertexShader = new Shader(VertexShader, "res/vert.glsl");
        this->fragShader = new Shader(FragmentShader, "res/frag.glsl");

        this->shaderProgram = new ShaderProgram();
        shaderProgram->attachShader(vertexShader->getId());
        shaderProgram->attachShader(fragShader->getId());
        shaderProgram->bind();

        shaderProgram->setUniform1i("TextureSlot", 0);

        this->drawer = new Drawer();

        this->model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        this->proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        this->view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));

        this->shaderProgram->setUniformMat4f("Model", this->model);
        this->shaderProgram->setUniformMat4f("View", this->view);
        this->shaderProgram->setUniformMat4f("Projection", this->proj);
        //shaderProgram->setUniform4f("Color", color.x, color.y, color.z, color.w);
        shaderProgram->setUniform1f("Scale", 0.5f);
	}

    void start() {
        this->renderDataBuffer = this->renderDataBufferStart;
        this->vertexCount = 0;
        this->IndexCount =  0;
    }

    void end() {
        this->vertexBuffer->pushData(this->renderDataBufferStart, sizeof(Vertex) * this->vertexCount);
        this->drawer->drawWithIdexes(this->vertexArray, IndexCount);
    }

    void drawQuad(float scale, glm::vec2 position, const char* textureName) {

        this->renderDataBuffer->position = { -scale + position.x, -scale + position.y, 0.0f };
        this->renderDataBuffer->textureCoords = { 0.0f, 0.0f };
        this->renderDataBuffer++;

        this->renderDataBuffer->position = { scale + position.x, -scale + position.y, 0.0f };
        this->renderDataBuffer->textureCoords = { 1.0f, 0.0f };
        this->renderDataBuffer++;

        this->renderDataBuffer->position = { scale + position.x, scale + position.y, 0.0f };
        this->renderDataBuffer->textureCoords = { 1.0f, 1.0f };
        this->renderDataBuffer++;

        this->renderDataBuffer->position = { -scale + position.x,  scale + position.y, 0.0f };
        this->renderDataBuffer->textureCoords = { 0.0f, 1.0f };
        this->renderDataBuffer++;
        
        this->vertexCount += 4;
        this->IndexCount += 6;
    }

    void createTexture(const char* filepath, const char* textureName) {
        this->textures[textureName] = new Texture(filepath);
        this->textures[textureName]->bind();
    }
};