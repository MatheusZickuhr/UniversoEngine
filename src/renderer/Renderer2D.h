#pragma once

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

namespace engine {

    const unsigned int maxQuads = 100;
    const unsigned int maxVertices = maxQuads * 4;
    const unsigned int maxIndices = maxQuads * 6;
    const unsigned int maxTextures = 32;

    struct Vertex {
        glm::vec3 position;
        glm::vec2 textureCoords;
        float textureSlot;
    };

    class Renderer2D {

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

        Renderer2D() {
            this->vertexCount = 0;
            this->IndexCount = 0;
            this->currentTextureSlot = 0;

            this->model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            this->proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
            this->view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));

            this->renderDataBufferStart = new Vertex[maxVertices];
            this->renderDataBuffer = this->renderDataBufferStart;

            this->vertexArray = std::make_shared<VertexArray>();
            this->createVetexBuffer();
            this->createIndexBuffer();

            this->loadShaders();
            this->setupShaderUniforms();

            this->drawer = std::make_unique<Drawer>();
        }

        ~Renderer2D() {
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

        void drawQuad(float scale, glm::vec2 position, const char* textureName) {

            auto texture = this->textures[textureName];

            float textureSlot = (float)texture->getSlot();

            this->renderDataBuffer->position = { -scale + position.x, -scale + position.y, 0.0f };
            this->renderDataBuffer->textureCoords = { 0.0f, 0.0f };
            this->renderDataBuffer->textureSlot = textureSlot;
            this->renderDataBuffer++;

            this->renderDataBuffer->position = { scale + position.x, -scale + position.y, 0.0f };
            this->renderDataBuffer->textureCoords = { 1.0f, 0.0f };
            this->renderDataBuffer->textureSlot = textureSlot;
            this->renderDataBuffer++;

            this->renderDataBuffer->position = { scale + position.x, scale + position.y, 0.0f };
            this->renderDataBuffer->textureCoords = { 1.0f, 1.0f };
            this->renderDataBuffer->textureSlot = textureSlot;
            this->renderDataBuffer++;

            this->renderDataBuffer->position = { -scale + position.x,  scale + position.y, 0.0f };
            this->renderDataBuffer->textureCoords = { 0.0f, 1.0f };
            this->renderDataBuffer->textureSlot = textureSlot;
            this->renderDataBuffer++;

            this->vertexCount += 4;
            this->IndexCount += 6;
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

    private:

        void createVetexBuffer() {
            this->vertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * maxVertices);
            this->vertexBuffer->addLayout(0, 3, sizeof(Vertex), offsetof(Vertex, position));
            this->vertexBuffer->addLayout(1, 2, sizeof(Vertex), offsetof(Vertex, textureCoords));
            this->vertexBuffer->addLayout(2, 1, sizeof(Vertex), offsetof(Vertex, textureSlot));
        }

        void createIndexBuffer() {
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

            this->indexBuffer = std::make_unique<IndexBuffer>(indices, maxIndices);
        }

        void loadShaders() {
            this->vertexShader = std::make_unique<Shader>(VertexShader, "res/shaders/2d/vert.glsl");
            this->fragShader = std::make_unique<Shader>(FragmentShader, "res/shaders/2d/frag.glsl");
            this->shaderProgram = std::make_unique<ShaderProgram>();
            shaderProgram->attachShader(vertexShader->getId());
            shaderProgram->attachShader(fragShader->getId());
            shaderProgram->bind();
        }

        void setupShaderUniforms() {
            int textureSlots[maxTextures];
            for (int i = 0; i < maxTextures; i++) textureSlots[i] = i;
            this->shaderProgram->setUniform1iv("TextureSlots", maxTextures, textureSlots);

            shaderProgram->setUniform1i("TextureSlot", 0);
            this->shaderProgram->setUniformMat4f("Model", this->model);
            this->shaderProgram->setUniformMat4f("View", this->view);
            this->shaderProgram->setUniformMat4f("Projection", this->proj);
            shaderProgram->setUniform1f("Scale", 0.5f);
        }

    };
}