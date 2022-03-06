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

        void drawDynamicMesh(Mesh* mesh, Material* material, const glm::mat4& transform, const unsigned int& renderId);

        void drawStaticMesh(Mesh* mesh, Material* material, const glm::mat4& transform, const unsigned int& renderId);

        void destroyStaticMesh(const unsigned int& renderId);

        void clearColor(float r, float g, float b, float a);

        unsigned int getDrawCallsCount();

        void setSkyBoxCubeMap(std::shared_ptr<CubeMap> skyBoxCubeMap);

    private:

        struct DynamicMeshData {
            Mesh* mesh;
            Material* material;
            glm::mat4 transform;
            unsigned int renderId;
        };

        struct StaticMeshData {
            Mesh* mesh;
            Material* material;
            glm::mat4 transform;
            unsigned int renderId;

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

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
            std::shared_ptr<CubeMap> cubeMap = nullptr;

            VertexArray vertexArray;
            VertexBuffer vertexBuffer{ sizeof(glm::vec3), 36 };

            ShaderProgram shaderProgram;
            Shader vertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/skyboxVertex.glsl" };
            Shader fragmentShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/skyboxFragment.glsl" };
        };

        struct DynamicRenderingData {
            const unsigned int maxVertices = 10000;
            const unsigned int maxIndices  = 20000;
            
            // current frame data (this data will reset on every frame)
            std::vector<DynamicMeshData> meshDataList;

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
        
            unsigned int currentTextureSlot;
            unsigned int currentCubeMapSlot;

            // starts on slot 1, 0 is reserved to draw meshes 
            const unsigned int initialTextureSlot = 1;
            const unsigned int initialCubeMapSlot = 0;

            std::vector<std::shared_ptr<Texture>> boundTextures;
            std::vector<std::shared_ptr<CubeMap>> boundCubeMaps;
        };

        const static unsigned int TEXTURE_SLOT = 0;

        const float NO_TEXTURE_SLOT = -1.0f;

        unsigned int drawCallsCount;
        
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