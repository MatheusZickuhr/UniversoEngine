#include "Renderer2D.h"

#include <algorithm>

#include "../debug/Assert.h"

namespace engine {

    Renderer2D::Renderer2D() {
        this->verticesBegin = new QuadVertex[maxQuadVertices];
        this->vertices = this->verticesBegin;

        this->vertexArray.addVertexBuffer(this->vertexBuffer);
        this->vertexArray.addIndexBuffer(this->indexBuffer);

        this->vertexBuffer.addAttributePointer(AttriuteType::Vec3, offsetof(QuadVertex, position));
        this->vertexBuffer.addAttributePointer(AttriuteType::Vec2, offsetof(QuadVertex, textureCoords));
        this->vertexBuffer.addAttributePointer(AttriuteType::Float, offsetof(QuadVertex, textureSlot));

        unsigned int* indices = new unsigned int[maxQuadIndices];
        unsigned int offset = 0;

        for (int i = 0; i < maxQuadIndices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        this->indexBuffer.pushData(indices, sizeof(unsigned int) * maxQuadIndices);

        delete[] indices;

        shaderProgram.attachShader(vertexShader);
        shaderProgram.attachShader(fragShader);
        shaderProgram.bind();

    }

    Renderer2D::~Renderer2D() {
        delete[] this->verticesBegin;
    }

    void Renderer2D::startFrame(Camera& camera) {
        ASSERT(!this->drawingStarted, "You need to call endFrame before calling startFrame angain");
        drawingStarted = true;

        int currentViewPortWidth = DrawApi::getViewPortWidth();
        int currentViewPortHeight = DrawApi::getViewPortHeight();

        CameraUniformBufferData cameraUniformBufferData{ camera.getViewProjectionMatrix(currentViewPortWidth, currentViewPortHeight) };

        this->cameraUniformBuffer.pushData(&cameraUniformBufferData, sizeof(CameraUniformBufferData));
    }

    void Renderer2D::endFrame() {
        ASSERT(this->drawingStarted, "You need to call startFrame before calling endFrame");

        drawingStarted = false;

        this->performDrawcall();

        this->boundTextures.clear();
        this->currentTextureSlot = 0;
    }

    void Renderer2D::addQuad(Texture* texture, glm::mat4 transform) {
        ASSERT(this->drawingStarted, "You need to call startFrame before calling addQuad");

        if (this->vertexCount + 4 > maxQuadVertices)
            this->performDrawcall();

        this->bindTexture(texture);

        this->vertices->position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 0.0f, 0.0f };
        this->vertices->textureSlot = texture->getSlot();
        this->vertices++;

        this->vertices->position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 1.0f, 0.0f };
        this->vertices->textureSlot = texture->getSlot();
        this->vertices++;

        this->vertices->position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 1.0f, 1.0f };
        this->vertices->textureSlot = texture->getSlot();
        this->vertices++;

        this->vertices->position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
        this->vertices->textureCoords = { 0.0f, 1.0f };
        this->vertices->textureSlot = texture->getSlot();
        this->vertices++;


        this->vertexCount += 4;
        this->indexCount += 6;
    }

    void Renderer2D::performDrawcall() {
        this->cameraUniformBuffer.bind(0);
        this->shaderProgram.bind();
        this->vertexArray.bind();

        this->vertexBuffer.pushData(this->verticesBegin, sizeof(QuadVertex) * this->vertexCount);
        DrawApi::drawWithIdexes(indexCount);

        this->vertices = this->verticesBegin;
        this->vertexCount = 0;
        this->indexCount = 0;
    }

    void Renderer2D::bindTexture(Texture* texture) {
        ASSERT(currentTextureSlot + 1 < Texture::maxTextures, "Maximum texture slot exceded");

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
