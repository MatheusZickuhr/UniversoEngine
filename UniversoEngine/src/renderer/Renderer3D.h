#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/IndexBuffer.h"
#include "renderer_api/Shader.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/DrawApi.h"
#include "renderer_api/Texture.h"
#include "Mesh.h"

namespace engine {

    // arbitrary values for now
    const unsigned int maxVertices = 10000;
    const unsigned int maxIndices  = 10000;

    class Renderer3D {

    public:

        Renderer3D();

        ~Renderer3D();

        void startDrawing(glm::mat4 mvp);

        void endDrawing();

        void drawMesh(Mesh* mesh, Texture* texture, glm::mat4 transform);

        void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

        void setViewPortSize(float width, float height);

        unsigned int getDrawCallsCount();

    private:

        unsigned int vertexCount    = 0;
        unsigned int indexCount     = 0;
        unsigned int drawCallsCount = 0;

        // always points to the begining of the vertex buffer
        Vertex* verticesBegin;
        Vertex* vertices;
        unsigned int* indices;

        VertexArray vertexArray;
        VertexBuffer vertexBuffer { sizeof(Vertex), maxVertices };
        IndexBuffer indexBuffer { maxIndices };

        Shader vertexShader { ShaderType::VertexShader, "res/shaders/3d/vert.glsl" };
        Shader fragShader { ShaderType::FragmentShader, "res/shaders/3d/frag.glsl" };
        ShaderProgram shaderProgram;

        DrawApi drawApi;

        void performDrawCall();
    };
}