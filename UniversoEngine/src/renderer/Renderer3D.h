#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vertex.h"
#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/IndexBuffer.h"
#include "renderer_api/Shader.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/DrawApi.h"
#include "renderer_api/Texture.h"
#include "renderer_api/FrameBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "Lighting.h"
#include "DrawCallBuffer.h"

namespace engine {

    // arbitrary values for now
    const unsigned int maxVertices = 10000;
    const unsigned int maxIndices  = 10000;

   
    class Renderer3D {

    public:

        Renderer3D();
        
        void startDrawing(glm::mat4 mvp, glm::vec3 cameraPosition, const float width, const float height);

        void endDrawing();

        void drawMesh(Mesh* mesh, Material* material, glm::mat4 transform);

        void startLightsDrawing();

        void endLightsDrawing();

        void drawPointLight(PointLight light, glm::mat4 transform);

        void drawDirectionalLight(DirectionalLight light);

        void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

        void setViewPortSize(float width, float height);

        unsigned int getDrawCallsCount();

    private:

        unsigned int drawCallsCount = 0;

        std::vector<Texture*> bindedTextures;

        unsigned int currentTextureSlot = 0;

        std::vector<DrawCallBuffer*> drawCallBuffers;

        DrawCallBuffer* currentDrawCallBuffer  = nullptr;

        VertexArray vertexArray;
        VertexBuffer vertexBuffer { sizeof(Vertex), maxVertices };
        IndexBuffer indexBuffer { maxIndices };

        Shader vertexShader { ShaderType::VertexShader, "res/shaders/3d/vert.glsl" };
        Shader fragShader { ShaderType::FragmentShader, "res/shaders/3d/frag.glsl" };
        ShaderProgram shaderProgram;

        // lighting
        std::vector<PointLight> pointLights;
        std::vector<DirectionalLight> directionalLights;

        // shadows
        ShaderProgram depthshaderProgram;
        Shader depthVertexShader{ ShaderType::VertexShader, "res/shaders/3d/depthVert.glsl" };
        Shader depthFragShader{ ShaderType::FragmentShader, "res/shaders/3d/depthFrag.glsl" };

        void render();
        
        void clearDrawCallBuffers();

        void updatePointLightsUniforms();

        void updateDirectionalLightsUniforms();

        void updateDepthBuffers();

        void bindTexture(Texture* texture);

        void clearBindedTextures();
    };
}