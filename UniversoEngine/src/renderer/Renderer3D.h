#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/IndexBuffer.h"
#include "renderer_api/Shader.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/Drawer.h"
#include "renderer_api/Texture.h"
#include "Mesh.h"

namespace engine {

    // arbitrary values for now
    const unsigned int maxVertices = 10000;
    const unsigned int maxIndices = 10000;

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

        Renderer3D();

        ~Renderer3D();

        void startDrawing(glm::mat4 mvp);

        void endDrawing();

        void drawMesh(
		    Mesh* mesh,
		    Texture* texture,
            glm::vec3 position,
            glm::vec3 scale,
            glm::vec3 rotationAxis,
            float rotationAngle
        );

        void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    private:

        void createVetexBuffer();

        void createIndexBuffer();

        void loadShaders();

        void setupShaderUniforms();
    };
}