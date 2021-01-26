#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/IndexBuffer.h"
#include "renderer_api/Shader.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/Texture.h"
#include "renderer_api/Drawer.h"
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
    unsigned int indexCount;

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
        this->indexCount = 0;

        this->verticesPtrStart = new Vertex[maxVertices];
        this->vertices = this->verticesPtrStart;
        this->indices = new unsigned int[maxIndices];

        this->vertexArray = std::make_shared<VertexArray>();
        this->createVetexBuffer();
        this->createIndexBuffer();

        this->loadShaders();

        this->setupShaderUniforms();


        this->drawer = std::make_unique<Drawer>();
    }

    ~Renderer3D() {
        delete[] this->verticesPtrStart;
        delete[] this->indices;
    }

    void startDrawing() {
        this->vertices = this->verticesPtrStart;
        this->vertexCount = 0;
        this->indexCount = 0;
    }

    void endDrawing() {
        this->vertexBuffer->pushData(this->verticesPtrStart, sizeof(Vertex) * this->vertexCount);
        this->indexBuffer->pushData(this->indices, sizeof(unsigned int) * this->indexCount);
        this->drawer->drawWithIdexes(this->vertexArray, this->indexCount);
    }

    void drawMesh(Mesh* mesh, Texture* texture, float scale, glm::vec3 position) {
        
        float textureSlot;

        if (texture != nullptr) {
            textureSlot = texture->getSlot();
            texture->bind();
        }
        else {
            textureSlot = -1.0f;
        }
            
        for (const Vertex vertex : mesh->vertices) {
            this->vertices->position = (vertex.position + position) * scale;
            this->vertices->textureCoords = vertex.textureCoords;
            this->vertices->textureSlot = textureSlot;
            this->vertices++;
        }

        for (int i = this->indexCount; i < mesh->vertices.size() + this->indexCount; i++) {
            this->indices[i] = i;
        }
        
        this->vertexCount += mesh->vertices.size();
        this->indexCount += mesh->vertices.size();
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
        this->vertexBuffer->addLayout(1, 2, sizeof(Vertex), offsetof(Vertex, textureCoords));
        this->vertexBuffer->addLayout(2, 1, sizeof(Vertex), offsetof(Vertex, textureSlot));
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

    void setupShaderUniforms() {
       
        const unsigned int maxTextures = 32;
        int textureSlots[maxTextures];
        for (int i = 0; i < maxTextures; i++) textureSlots[i] = i;
        this->shaderProgram->setUniform1iv("textureSlots", maxTextures, textureSlots);
    }
};