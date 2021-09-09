#pragma once

#include <vector>

#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/IndexBuffer.h"
#include "renderer_api/Shader.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/DrawApi.h"
#include "renderer_api/Texture.h"
#include "renderer_api/FrameBuffer.h"
#include "renderer_api/CubeMap.h"
#include "renderer_api/UniformBuffer.h"

#include "Vertex.h"
#include "Material.h"
#include "Mesh.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Camera.h"

namespace engine {

    class Renderer3D {

    public:

        struct MeshData {
            Mesh* mesh;
            Material* material;
            glm::mat4 transform;
        };
        
        Renderer3D();

        Renderer3D(Renderer3D const& other) = delete;

        ~Renderer3D();

        void beginFrame(Camera& camera);

        void endFrame();

        void addPointLight(PointLight light, glm::mat4 transform);

        void addDirectionalLight(DirectionalLight light, glm::mat4 transform);

        void drawDynamicMesh(MeshData meshData) { dynamicRenderingData.meshDataList.push_back(meshData); }

        void drawStaticMesh(MeshData meshData);

        void clearColor(float r, float g, float b, float a) { DrawApi::clearColor(r, g, b, a); }

        void setViewPortSize(int width, int height) { DrawApi::setViewPortSize(width, height); }

        unsigned int getDrawCallsCount() { return this->drawCallsCount; }

        void setSkyBoxCubeMap(CubeMap* skyBoxCubeMap) { this->skyBoxData.cubeMap = skyBoxCubeMap; }

    private:

        struct CameraUniformBufferData {
            glm::mat4 viewProjectionMatrix;
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
            glm::vec3 position;
        };

        struct alignas(16) PointLightUniformBufferData {
            glm::vec4 position;
            glm::vec4 ambient;
            glm::vec4 diffuse;
            glm::vec4 specular;
            float constant;
            float linear;
            float quadratic;
            float farPlane;
            int cubeMapSlotIndex;
        };

        struct alignas(16) DirectionalLightUniformBufferData {
            glm::mat4 viewProjection;
            glm::vec4 position;
            glm::vec4 ambient;
            glm::vec4 diffuse;
            glm::vec4 specular;
            int textureSlotIndex;
        };

        struct LightsUniformBufferData {
            PointLightUniformBufferData pointLights[PointLight::maxPointLights];
            DirectionalLightUniformBufferData directionalLights[DirectionalLight::maxDirectionalLights];
            int numberOfPointLights;
            int numberOfDirectionalLights;
        };

        struct CurrentPointLightUniformBufferData {
            glm::mat4 shadowMatrices[6];
            glm::vec4 lightPosition;
            float farPlane;
        };

        struct CurrentDirectionalLightUniformBufferData {
            glm::mat4 lightSpaceMatrix;
        };

        struct SkyBoxData {
            CubeMap* cubeMap = nullptr;

            VertexArray vertexArray;
            VertexBuffer vertexBuffer{ sizeof(glm::vec3), 36 };

            ShaderProgram shaderProgram;
            Shader vertexShader{ ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/skyboxVertex.glsl" };
            Shader fragmentShader{ ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/skyboxFragment.glsl" };
        };

        struct DynamicRenderingData {
            const unsigned int maxVertices = 10000;
            const unsigned int maxIndices  = 20000;
            
            // current frame data (this data will reset on every frame)
            std::vector<MeshData> meshDataList;

            // current batch data (this data will reset on every new batch)
            unsigned int vertexCount = 0;
            unsigned int indexCount  = 0;

            Vertex* vertices;
            Vertex* verticesBegin;
            unsigned int* indices;

            VertexArray vertexArray;
            VertexBuffer vertexBuffer{ sizeof(Vertex), maxVertices };
            IndexBuffer indexBuffer{ maxIndices };
        };

        struct StaticRenderingData {
            bool shouldCreateBuffers = false;

            std::vector<MeshData> meshDataList;

            unsigned int vertexCount = 0;
            unsigned int indexCount = 0;

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            VertexArray vertexArray;
            VertexBuffer* vertexBuffer = nullptr;
            IndexBuffer* indexBuffer = nullptr;
        };

        unsigned int drawCallsCount = 0;
        
        DynamicRenderingData dynamicRenderingData;
        StaticRenderingData staticRenderingData;

        std::vector<Texture*> boundTextures;
        unsigned int currentTextureSlot = 0;

        std::vector<Texture*> boundCubeMaps;
        unsigned int currentCubeMapSlot = 0;


        Shader vertexShader { ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/lightingVertex.glsl" };
        Shader fragmentShader { ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/lightingFragment.glsl" };
        ShaderProgram shaderProgram;

        UniformBuffer cameraUniformBuffer { sizeof(CameraUniformBufferData) };

        // lighting
        std::vector<PointLight> pointLights;
        std::vector<DirectionalLight> directionalLights;

        UniformBuffer lightsUniformBuffer{ sizeof(LightsUniformBufferData) };
        UniformBuffer currentPointLightUniformBuffer{ sizeof(CurrentPointLightUniformBufferData) };
        UniformBuffer currentDirectionalLightUniformBuffer{ sizeof(CurrentDirectionalLightUniformBufferData) };

        ShaderProgram depthTextureShaderProgram;
        Shader depthTextureVertexShader{ ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/depthMapVertex.glsl" };
        Shader depthTextureFragmentShader{ ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/depthMapFragment.glsl" };

        ShaderProgram depthCubeMapShaderProgram;
        Shader depthCubeMapVertexShader { ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/cubeMapDepthMapVertex.glsl" };
        Shader depthCubeMapGeometryShader { ShaderType::GeometryShader, "UniversoEngine/resources/shaders/3d/cubeMapDepthMapGeometry.glsl" };
        Shader depthCubeMapFragmentShader { ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/cubeMapDepthMapFragment.glsl" };

        SkyBoxData skyBoxData;

        void drawStaticMeshes(ShaderProgram* targetShaderProgram, FrameBuffer* frameBufferTarget = nullptr);

        void drawDynamicMeshes(ShaderProgram* targetShaderProgram, FrameBuffer* frameBufferTarget = nullptr);

        void drawLightsFrameBuffers();

        void bindUniformBuffers();

        void bindTexture(Texture* texture);

        void bindCubeMap(Texture* texture);
                
        void flushDynamicRenderingBatch();

        void updateCameraUniformBuffer(Camera& camera);

        void clearLightsFrameBuffers();

        void updateLightsUniformBuffers();

        void drawSkyBox();

        void clearLights();

        void clearBindedTextures();

    };
}