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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Camera.h"

namespace engine {

    // arbitrary values for now
    const unsigned int maxVertices = 10000;
    const unsigned int maxIndices  = 10000;
   
    class Renderer3D {

    public:

        Renderer3D();

        ~Renderer3D();
        
        void startDrawing(Camera& camera);

        void endDrawing();

        void drawMesh(Mesh* mesh, Material* material, glm::mat4 transform);

        void startLightsDrawing();
        
        void endLightsDrawing();

        void drawMeshShadowMap(Mesh* mesh, glm::mat4 transform);

        void drawPointLight(PointLight light, glm::mat4 transform);

        void drawDirectionalLight(DirectionalLight light, glm::mat4 transform);

        void clearColor(float r, float g, float b, float a);

        void setViewPortSize(int width, int height);

        unsigned int getDrawCallsCount();

    private:

        unsigned int drawCallsCount = 0;

        std::vector<Texture*> bindedTextures;
        unsigned int currentTextureSlot = 0;

        std::vector<Texture*> bindedCubeMaps;
        unsigned int currentCubeMapSlot = 0;

        Vertex* vertices;
        Vertex* verticesBegin;
        unsigned int* indices;

        unsigned int vertexCount = 0;
        unsigned int indexCount =  0;

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

        ShaderProgram cubeMapDepthMapShaderProgram;
        Shader cubeMapDepthMapVertexShader { ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/cubeMapDepthMapVertex.glsl" };
        Shader cubeMapDepthMapGeometryShader { ShaderType::GeometryShader, "UniversoEngine/resources/shaders/3d/cubeMapDepthMapGeometry.glsl" };
        Shader cubeMapDepthMapFragmentShader { ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/cubeMapDepthMapFragment.glsl" };

        void bindTexture(Texture* texture);

        void bindCubeMap(Texture* texture);

        void clearBindedTextures();

        void clearBindedCubeMaps();
                
        void performDrawCall();

        void performShadowMapDrawCalls();

    };
}