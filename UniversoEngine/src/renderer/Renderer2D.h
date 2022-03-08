#pragma once

#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/IndexBuffer.h"
#include "renderer_api/Shader.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/Texture.h"
#include "renderer_api/DrawApi.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <array>
#include <vector>
#include "renderer_api/UniformBuffer.h"

namespace engine {

    class Renderer2D {

    public:

        Renderer2D();

        Renderer2D(Renderer2D const& other) = delete;

        ~Renderer2D();

        void startFrame(Camera& camera);

        void endFrame();

        void addQuad(Texture* texture, glm::mat4 transform);

        void clearColor(float r, float g, float b, float a) { DrawApi::clearColor(r, g, b, a); }

    private:

        struct QuadVertex {
            glm::vec3 position;
            glm::vec2 textureCoords;
            float textureSlot;
        };

        struct CameraUniformBufferData {
            glm::mat4 cameraViewProjection;
        };

        const uint32_t maxQuads        = 100;
        const uint32_t maxQuadVertices = 4 * maxQuads;
        const uint32_t maxQuadIndices  = 6 * maxQuads;

        std::vector<Texture*> boundTextures;

        uint32_t currentTextureSlot = 0;

        QuadVertex* verticesBegin;
        QuadVertex* vertices;

        VertexArray vertexArray;
        VertexBuffer vertexBuffer{ sizeof(QuadVertex), maxQuadVertices };
        IndexBuffer indexBuffer{ maxQuadIndices };

        UniformBuffer cameraUniformBuffer{ sizeof(CameraUniformBufferData) };

        Shader vertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/2d/vertex.glsl" };
        Shader fragShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/2d/fragment.glsl" };
        ShaderProgram shaderProgram;

        bool drawingStarted = false;

        uint32_t vertexCount = 0;
        uint32_t indexCount =  0;

        void performDrawcall();

        void bindTexture(Texture* texture);
    };

}