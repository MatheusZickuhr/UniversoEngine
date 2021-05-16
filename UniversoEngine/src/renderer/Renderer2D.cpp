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

        unsigned int indices[maxQuadIndices];
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

        shaderProgram.attachShader(vertexShader);
        shaderProgram.attachShader(fragShader);
        shaderProgram.bind();

    }

    Renderer2D::~Renderer2D() {
        delete[] this->verticesBegin;
    }

    void Renderer2D::startDrawing(Camera& camera) {
        ASSERT(!this->drawingStarted, "You need to call endDrawing before calling startDrawing angain");
        drawingStarted = true;

        int currentViewPortWidth = DrawApi::getViewPortWidth();
        int currentViewPortHeight = DrawApi::getViewPortHeight();

        this->shaderProgram.setMat4Uniform("viewProjection", camera.getViewProjectionMatrix(currentViewPortWidth, currentViewPortHeight));
    }

    void Renderer2D::endDrawing() {
        ASSERT(this->drawingStarted, "You need to call startDrawing before calling endDrawing");

        drawingStarted = false;

        this->performDrawcall();

        this->clearBindedTextures();
    }

    void Renderer2D::drawQuad(Texture* texture, glm::mat4 transform) {
        ASSERT(this->drawingStarted, "You need to call startDrawing before calling drawQuad");

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

    void Renderer2D::clearColor(float r, float g, float b, float a) {
        DrawApi::clearColor(r, g, b, a);
    }

    void Renderer2D::performDrawcall() {
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

        bool textureBinded = std::find(this->bindedTextures.begin(), this->bindedTextures.end(), texture)
            != this->bindedTextures.end();

        if (!textureBinded) {
            texture->bind(this->currentTextureSlot);
            this->bindedTextures.push_back(texture);
            this->currentTextureSlot++;
        }
    }

    void Renderer2D::clearBindedTextures() {
        this->bindedTextures.clear();
        this->currentTextureSlot = 0;
    }
}
