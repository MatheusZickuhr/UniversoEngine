#pragma once

#include <vector>
#include <memory>

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

        Renderer3D();

        Renderer3D(Renderer3D const& other) = delete;

        ~Renderer3D();

        void beginFrame(Camera& camera);

        void endFrame();

        void drawPointLight(const PointLight& light);

        void drawDirectionalLight(const DirectionalLight& light);

        void drawDynamicMesh(Mesh* mesh, Material* material, const glm::mat4& transform, const uint32_t& renderId);

        void drawStaticMesh(Mesh* mesh, Material* material, const glm::mat4& transform, const uint32_t& renderId);

        void destroyStaticMesh(const uint32_t& renderId);

        void clearColor(float r, float g, float b, float a);

        uint32_t getDrawCallsCount();

        void setSkyBoxCubeMap(std::shared_ptr<CubeMap> skyBoxCubeMap);

    private:

        struct DynamicMeshData {
            Mesh* mesh;
            Material* material;
            glm::mat4 transform;
            uint32_t renderId;
        };

        struct StaticMeshData {
            Mesh* mesh;
            Material* material;
            glm::mat4 transform;
            uint32_t renderId;

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            std::shared_ptr<VertexArray> vertexArray;
            std::shared_ptr<VertexBuffer> vertexBuffer;
            std::shared_ptr<IndexBuffer> indexBuffer;
        };

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
            int32_t cubeMapSlotIndex;
        };

        struct alignas(16) DirectionalLightUniformBufferData {
            glm::mat4 viewProjection;
            glm::vec4 position;
            glm::vec4 ambient;
            glm::vec4 diffuse;
            glm::vec4 specular;
            int32_t textureSlotIndex;
        };

        struct LightsUniformBufferData {
            PointLightUniformBufferData pointLights[PointLight::maxPointLights];
            DirectionalLightUniformBufferData directionalLights[DirectionalLight::maxDirectionalLights];
            int32_t numberOfPointLights;
            int32_t numberOfDirectionalLights;
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
            std::shared_ptr<CubeMap> cubeMap = nullptr;

            VertexArray vertexArray;
            VertexBuffer vertexBuffer{ sizeof(glm::vec3), 36 };

            ShaderProgram shaderProgram;
            Shader vertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/skyboxVertex.glsl" };
            Shader fragmentShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/skyboxFragment.glsl" };
        };

        struct DynamicRenderingData {
            const uint32_t maxVertices = 10000;
            const uint32_t maxIndices  = 20000;
            
            // current frame data (this data will reset on every frame)
            std::vector<DynamicMeshData> meshDataList;

            // current batch data (this data will reset on every new batch)
            uint32_t vertexCount = 0;
            uint32_t indexCount  = 0;

            Vertex* vertices = nullptr;
            Vertex* verticesBegin = nullptr;
            uint32_t* indices = nullptr;

            VertexArray vertexArray;
            VertexBuffer vertexBuffer{ sizeof(Vertex), maxVertices };
            IndexBuffer indexBuffer{ maxIndices };
        };

        struct StaticRenderingData {

            std::vector<StaticMeshData> meshDataList;

        };

        struct LightingData {
            std::vector<PointLight> pointLights;
            std::vector<DirectionalLight> directionalLights;

            UniformBuffer lightsUniformBuffer{ sizeof(LightsUniformBufferData) };
            UniformBuffer currentPointLightUniformBuffer{ sizeof(CurrentPointLightUniformBufferData) };
            UniformBuffer currentDirectionalLightUniformBuffer{ sizeof(CurrentDirectionalLightUniformBufferData) };

            ShaderProgram depthTextureShaderProgram;
            Shader depthTextureVertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/depthMapVertex.glsl" };
            Shader depthTextureFragmentShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/depthMapFragment.glsl" };

            ShaderProgram depthCubeMapShaderProgram;
            Shader depthCubeMapVertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/cubeMapDepthMapVertex.glsl" };
            Shader depthCubeMapGeometryShader{ ShaderType::GeometryShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/cubeMapDepthMapGeometry.glsl" };
            Shader depthCubeMapFragmentShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/cubeMapDepthMapFragment.glsl" };
        
            // starts on slot 1, 0 is reserved to draw meshes 
            const uint32_t initialTextureSlot = 1;
            const uint32_t initialCubeMapSlot = 0;

            uint32_t currentTextureSlot = initialTextureSlot;
            uint32_t currentCubeMapSlot = initialCubeMapSlot;

            std::vector<std::shared_ptr<Texture>> boundTextures;
            std::vector<std::shared_ptr<CubeMap>> boundCubeMaps;
        };

        const static uint32_t TEXTURE_SLOT = 0;

        const float NO_TEXTURE_SLOT = -1.0f;

        uint32_t drawCallsCount;
        
        DynamicRenderingData dynamicRenderingData;
        StaticRenderingData staticRenderingData;
        LightingData lightingData;
        SkyBoxData skyBoxData;

        Shader vertexShader { ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/lightingVertex.glsl" };
        Shader fragmentShader { ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/lightingFragment.glsl" };
        ShaderProgram shaderProgram;

        UniformBuffer cameraUniformBuffer { sizeof(CameraUniformBufferData) };

        void updateLightsDepthBufferTextures();

        void drawStaticMeshes(ShaderProgram& targetShaderProgram, std::shared_ptr<FrameBuffer> frameBufferTarget = nullptr);

        void drawDynamicMeshes(ShaderProgram& targetShaderProgram, std::shared_ptr<FrameBuffer> frameBufferTarget = nullptr);

        void drawSkyBox();

        Vertex transformAndApplyMateriaToVertex(const Vertex& vertex, glm::mat4& transform, Material* material);

        void bindUniformBuffers();

        void bindLightingTexture(std::shared_ptr<Texture> texture);

        void bindLightingCubeMap(std::shared_ptr<CubeMap> cubeMap);
                
        void updateCameraUniformBuffer(Camera& camera);

        void updateLightsUniformBuffers();

        void clearLights();

        void clearLightingBoundTextures();

        void clearDynamicMeshes();

    };
}