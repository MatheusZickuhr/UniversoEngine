#include "Renderer2D.h"
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

        shaderProgram.attachShader(vertexShader.getId());
        shaderProgram.attachShader(fragShader.getId());
        shaderProgram.bind();

        int textureSlots[Texture::maxTextureSlot];
        for (int i = 0; i < Texture::maxTextureSlot; i++) textureSlots[i] = i;
        this->shaderProgram.setIntArrayUniform("TextureSlots", Texture::maxTextureSlot, textureSlots);

    }

    Renderer2D::~Renderer2D() {
        delete[] this->verticesBegin;
    }

    void Renderer2D::startDrawing(glm::mat4 mvp) {
        ASSERT(!this->drawingStarted, "You need to call endDrawing before calling startDrawing angain");

        drawingStarted = true;

        this->shaderProgram.setMat4Uniform("Mvp", mvp);
    }

    void Renderer2D::endDrawing() {
        ASSERT(this->drawingStarted, "You need to call startDrawing before calling endDrawing");

        drawingStarted = false;

        this->performDrawcall();
    }

    void Renderer2D::drawQuad(Texture* texture, glm::mat4 transform) {
        ASSERT(this->drawingStarted, "You need to call startDrawing before calling drawQuad");

        if (this->vertexCount + 4 > maxQuadVertices)
            this->performDrawcall();

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

    void Renderer2D::clear(float r, float g, float b, float a) {
        DrawApi::clear(r, g, b, a);
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
}
