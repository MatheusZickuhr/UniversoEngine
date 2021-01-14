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

const unsigned int maxCubes = 100;
const unsigned int maxVertices = maxCubes * 36;
const unsigned int maxIndices = maxCubes * 36;
const unsigned int maxTextures = 32;

struct Vertex {
    glm::vec3 position;
};

class Renderer3D {

private:
    std::shared_ptr<VertexArray> vertexArray;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<Shader> vertexShader;
    std::unique_ptr<Shader> fragShader;
    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<Drawer> drawer;
    std::map<const char*, std::shared_ptr<Texture>> textures;
    Vertex* renderDataBufferStart;
    Vertex* renderDataBuffer;
    unsigned int vertexCount;
    unsigned int IndexCount;
    unsigned int currentTextureSlot;
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;

public:

    Renderer3D() {
        this->vertexCount = 0;
        this->IndexCount = 0;
        this->currentTextureSlot = 0;

        this->model = glm::mat4(1.0f);
        this->proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);;
        this->view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)  );;

        this->renderDataBufferStart = new Vertex[maxVertices];
        this->renderDataBuffer = this->renderDataBufferStart;

        this->vertexArray = std::make_shared<VertexArray>();
        this->createVetexBuffer();
        this->createIndexBuffer();

        this->loadShaders();
        this->setupShaderUniforms();

        this->drawer = std::make_unique<Drawer>();
    }

    ~Renderer3D() {
        delete[] this->renderDataBufferStart;
    }

    void start() {
        this->renderDataBuffer = this->renderDataBufferStart;
        this->vertexCount = 0;
        this->IndexCount = 0;
    }

    void end() {
        this->vertexBuffer->pushData(this->renderDataBufferStart, sizeof(Vertex) * this->vertexCount);
        this->drawer->drawWithIdexes(this->vertexArray, IndexCount);
    }

    void drawCube(float scale, glm::vec3 position) {
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++;  
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++;  
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f,-1.0) + position) * scale;
        this->renderDataBuffer++;  
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++;  
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f,-1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(-1.0f, 1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 
        this->renderDataBuffer->position = (glm::vec3(1.0f,-1.0f, 1.0f) + position) * scale;
        this->renderDataBuffer++; 

        this->vertexCount += 36;
        this->IndexCount += 36;
    }

    void createTexture(const char* filepath, const char* textureName) {
        auto texture = std::make_shared<Texture>(filepath, this->currentTextureSlot);
        texture->bind();
        this->textures[textureName] = texture;
        this->currentTextureSlot++;
    }

private:

    void createVetexBuffer() {
        this->vertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * maxVertices);
        this->vertexBuffer->addLayout(0, 3, sizeof(Vertex), offsetof(Vertex, position));
    }

    void createIndexBuffer() {
        unsigned int indices[maxIndices];
        for (int i = 0; i < maxIndices; i++) indices[i] = i;
        this->indexBuffer = std::make_unique<IndexBuffer>(indices, maxIndices);
    }

    void loadShaders() {
        this->vertexShader = std::make_unique<Shader>(VertexShader, "res/shaders/3d/vert.glsl");
        this->fragShader = std::make_unique<Shader>(FragmentShader, "res/shaders/3d/frag.glsl");
        this->shaderProgram = std::make_unique<ShaderProgram>();
        shaderProgram->attachShader(vertexShader->getId());
        shaderProgram->attachShader(fragShader->getId());
        shaderProgram->bind();
    }

    void setupShaderUniforms() {
        this->shaderProgram->setUniformMat4f("Model", this->model);
        this->shaderProgram->setUniformMat4f("View", this->view);
        this->shaderProgram->setUniformMat4f("Projection", this->proj);
    }

};