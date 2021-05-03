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
#include <vector>

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

        void startDrawing(glm::mat4 viewProjection);

        void endDrawing();

        void drawQuad(Texture* texture, glm::mat4 transform);

        void clearColor(float r, float g, float b, float a);

    private:

        std::vector<Texture*> bindedTextures;

        unsigned int currentTextureSlot = 0;

        QuadVertex* verticesBegin;
        QuadVertex* vertices;

        VertexArray vertexArray;
        VertexBuffer vertexBuffer{ sizeof(QuadVertex), maxQuadVertices };
        IndexBuffer indexBuffer{ maxQuadIndices };

        Shader vertexShader{ ShaderType::VertexShader, "UniversoEngine/resources/shaders/2d/vertex.glsl" };
        Shader fragShader{ ShaderType::FragmentShader, "UniversoEngine/resources/shaders/2d/fragment.glsl" };
        ShaderProgram shaderProgram;

        bool drawingStarted = false;

        unsigned int vertexCount = 0;
        unsigned int indexCount =  0;

        void performDrawcall();

        void bindTexture(Texture* texture);

        void clearBindedTextures();
    };

}