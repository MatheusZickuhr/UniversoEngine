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

// arbitrary values for now
const unsigned int maxVertices = 10000;
const unsigned int maxIndices =  10000;

class Renderer3D {

private:
    unsigned int vertexCount;
    unsigned int IndexCount;
    unsigned int currentTextureSlot;

    Vertex* verticesPtrStart;
    Vertex* vertices;
    unsigned int* indices;

    std::shared_ptr<VertexArray> vertexArray;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;

    std::unique_ptr<Shader> vertexShader;
    std::unique_ptr<Shader> fragShader;
    std::unique_ptr<ShaderProgram> shaderProgram;

    std::unique_ptr<Drawer> drawer;
 
public:

    Renderer3D() {
        this->vertexCount = 0;
        this->IndexCount = 0;
        this->currentTextureSlot = 0;

        this->verticesPtrStart = new Vertex[maxVertices];
        this->vertices = this->verticesPtrStart;
        this->indices = new unsigned int[maxIndices];

        this->vertexArray = std::make_shared<VertexArray>();
        this->createVetexBuffer();
        this->createIndexBuffer();

        this->loadShaders();

        this->drawer = std::make_unique<Drawer>();
    }

    ~Renderer3D() {
        delete[] this->verticesPtrStart;
        delete[] this->indices;
    }

    void startDrawing() {
        this->vertices = this->verticesPtrStart;
        this->vertexCount = 0;
        this->IndexCount = 0;
    }

    void endDrawing() {
        this->vertexBuffer->pushData(this->verticesPtrStart, sizeof(Vertex) * this->vertexCount);
        this->indexBuffer->pushData(this->indices, sizeof(unsigned int) * IndexCount);
        this->drawer->drawWithIdexes(this->vertexArray, IndexCount);
    }

    void drawMesh(Mesh* mesh, float scale, glm::vec3 position) {
        for (const Vertex vertex : mesh->vertices) {
            this->vertices->position = (vertex.position + position) * scale;
            this->vertices++;
        }

        for (int i = this->IndexCount; i < mesh->vertices.size() + this->IndexCount; i++) {
            this->indices[i] = i;
        }
        
        this->vertexCount += mesh->vertices.size();
        this->IndexCount += mesh->vertices.size();
    }

    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
        this->drawer->clear(r, g, b, a);
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
        this->indexBuffer = std::make_unique<IndexBuffer>(maxIndices);
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