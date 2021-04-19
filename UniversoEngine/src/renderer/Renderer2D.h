#pragma once

#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/IndexBuffer.h"
#include "renderer_api/Shader.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/Texture.h"
#include "renderer_api/DrawApi.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <array>

namespace engine {

    const unsigned int maxQuads = 100;
    const unsigned int maxQuadVertices = 4 * maxQuads;
    const unsigned int maxQuadIndices =  6 * maxQuads;

    struct QuadVertex {
        glm::vec3 position;
        glm::vec2 textureCoords;
        float textureSlot;
    };

    class Renderer2D {

    public:

        Renderer2D();

        ~Renderer2D();

        void startDrawing(glm::mat4 mvp);

        void endDrawing();

        void drawQuad(Texture* texture, glm::mat4 transform);

        void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    private:

        QuadVertex* verticesBegin;
        QuadVertex* vertices;

        VertexArray vertexArray;
        VertexBuffer vertexBuffer{ sizeof(QuadVertex), maxQuadVertices };
        IndexBuffer indexBuffer{ maxQuadIndices };

        Shader vertexShader{ ShaderType::VertexShader, "res/shaders/2d/vert.glsl" };
        Shader fragShader{ ShaderType::FragmentShader, "res/shaders/2d/frag.glsl" };
        ShaderProgram shaderProgram;

        DrawApi drawApi;

        bool drawingStarted = false;

        unsigned int vertexCount = 0;
        unsigned int indexCount =  0;

        void performDrawcall();
    };

}