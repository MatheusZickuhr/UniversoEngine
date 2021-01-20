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
#include "Vertex.h"
#include "Mesh.h"

const unsigned int cubeVertexCount = 36;
const unsigned int maxCubes = 100;
const unsigned int maxVertices = maxCubes * cubeVertexCount;
const unsigned int maxIndices = maxCubes * cubeVertexCount;
const unsigned int maxTextures = 32;


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
    std::unique_ptr<Mesh> cubeMesh;
    unsigned int vertexCount;
    unsigned int IndexCount;
    unsigned int currentTextureSlot;


public:

    Renderer3D() {
        this->vertexCount = 0;
        this->IndexCount = 0;
        this->currentTextureSlot = 0;

        this->renderDataBufferStart = new Vertex[maxVertices];
        this->renderDataBuffer = this->renderDataBufferStart;

        this->vertexArray = std::make_shared<VertexArray>();
        this->createVetexBuffer();
        this->createIndexBuffer();

        this->loadShaders();

        this->drawer = std::make_unique<Drawer>();

        this->cubeMesh = std::make_unique<Mesh>("res/models/cube/cube.obj");
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
        for (const Vertex vertex : this->cubeMesh->vertices) {
            this->renderDataBuffer->position = (vertex.position + position) * scale;
            this->renderDataBuffer++;
        }
        
        this->vertexCount += cubeVertexCount;
        this->IndexCount += cubeVertexCount;
    }

    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
        this->drawer->clear(r, g, b, a);
    }

    void createTexture(const char* filepath, const char* textureName) {
        auto texture = std::make_shared<Texture>(filepath, this->currentTextureSlot);
        texture->bind();
        this->textures[textureName] = texture;
        this->currentTextureSlot++;
    }

    void setModelViewProjectionMatrix(glm::mat4 mvp) {
        this->shaderProgram->setUniformMat4f("Mvp", mvp);
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

};