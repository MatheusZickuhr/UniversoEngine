#include "Renderer2d.h"

#include <algorithm>

#include "../debug/Assert.h"

namespace engine {

    Renderer2d::Renderer2d() {
        this->verticesBegin = new QuadVertex[maxQuadVertices];
        this->vertices = this->verticesBegin;

        this->vertexArray.addVertexBuffer(this->vertexBuffer);
        this->vertexArray.addIndexBuffer(this->indexBuffer);

        this->vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(QuadVertex, position));
        this->vertexBuffer.addAttributePointer(AttriuteType::Vec2, offsetof(QuadVertex, textureCoords));
        this->vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(QuadVertex, textureSlot));

        uint32_t* indices = new uint32_t[maxQuadIndices];
        uint32_t offset = 0;

        for (uint32_t i = 0; i < maxQuadIndices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        this->indexBuffer.pushData(indices, sizeof(uint32_t) * maxQuadIndices);

        delete[] indices;

        shaderProgram.attachShader(vertexShader);
        shaderProgram.attachShader(fragShader);
        shaderProgram.bind();

    }

    Renderer2d::~Renderer2d() {
        delete[] this->verticesBegin;
    }

    void Renderer2d::startFrame(std::shared_ptr<Camera> camera) {
        ASSERT(!this->drawingStarted, "You need to call endFrame before calling startFrame angain");
        drawingStarted = true;

       float width = (float) DrawApi::getViewPortWidth();
       float height = (float) DrawApi::getViewPortHeight();

        CameraUniformBufferData cameraUniformBufferData{ camera->getViewProjectionMatrix(width, height) };

        this->cameraUniformBuffer.pushData(&cameraUniformBufferData, sizeof(CameraUniformBufferData));
    }

    void Renderer2d::endFrame() {
        ASSERT(this->drawingStarted, "You need to call startFrame before calling endFrame");

        drawingStarted = false;

        this->performDrawcall();

        this->boundTextures.clear();
        this->currentTextureSlot = 0;
    }

    void Renderer2d::addQuad(Texture* texture, glm::mat4 transform) {
        ASSERT(this->drawingStarted, "You need to call startFrame before calling addQuad");

        if (this->vertexCount + 4 > maxQuadVertices)
            this->performDrawcall();

        this->bindTexture(texture);

        this->vertices->position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 0.0f, 0.0f };
        this->vertices->textureSlot = (float) texture->getSlot();
        this->vertices++;

        this->vertices->position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 1.0f, 0.0f };
        this->vertices->textureSlot = (float) texture->getSlot();
        this->vertices++;

        this->vertices->position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 1.0f, 1.0f };
        this->vertices->textureSlot = (float) texture->getSlot();
        this->vertices++;

        this->vertices->position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 0.0f, 1.0f };
        this->vertices->textureSlot = (float) texture->getSlot();
        this->vertices++;


        this->vertexCount += 4;
        this->indexCount += 6;
    }

    void Renderer2d::performDrawcall() {
        this->cameraUniformBuffer.bind(0);
        this->shaderProgram.bind();
        this->vertexArray.bind();

        this->vertexBuffer.pushData(this->verticesBegin, sizeof(QuadVertex) * this->vertexCount);
        DrawApi::drawWithIdexes(indexCount);

        this->vertices = this->verticesBegin;
        this->vertexCount = 0;
        this->indexCount = 0;
    }

    void Renderer2d::bindTexture(Texture* texture) {
        ASSERT(currentTextureSlot + 1 < Texture::MAX_TEXTURES, "Maximum texture slot exceded");

        if (texture == nullptr) return;

        bool textureBound = std::find(this->boundTextures.begin(), this->boundTextures.end(), texture)
            != this->boundTextures.end();

        if (!textureBound) {
            texture->bind(this->currentTextureSlot);
            this->boundTextures.push_back(texture);
            this->currentTextureSlot++;
        }
    }

}
