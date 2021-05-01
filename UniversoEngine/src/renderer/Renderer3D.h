#pragma once

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
#include "Camera.h"

namespace engine {

    // arbitrary values for now
    const unsigned int maxVertices = 10000;
    const unsigned int maxIndices  = 10000;

   
    class Renderer3D {

    public:

        Renderer3D();
        
        void startDrawing(Camera& camera, const float width, const float height);

        void endDrawing();

        void drawMesh(Mesh* mesh, Material* material, glm::mat4 transform);

        void startLightsDrawing();

        void endLightsDrawing();

        void drawPointLight(PointLight light, glm::mat4 transform);

        void drawDirectionalLight(DirectionalLight light);

        void clearColor(float r, float g, float b, float a);

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

        Shader vertexShader { ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/lightingVertex.glsl" };
        Shader fragShader { ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/lightingFragment.glsl" };
        ShaderProgram shaderProgram;

        // lighting
        std::vector<PointLight> pointLights;
        std::vector<DirectionalLight> directionalLights;

        // shadows
        ShaderProgram depthshaderProgram;
        Shader depthVertexShader{ ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/depthMapVertex.glsl" };
        Shader depthFragShader{ ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/depthMapFragment.glsl" };

        void render();
        
        void clearDrawCallBuffers();

        void updatePointLightsUniforms();

        void updateDirectionalLightsUniforms();

        void updateDepthBuffers();

        void bindTexture(Texture* texture);

        void clearBindedTextures();
    };
}