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
            unsigned int renderId;
        };
        
        Renderer3D();

        Renderer3D(Renderer3D const& other) = delete;

        ~Renderer3D();

        void beginFrame(Camera& camera);

        void endFrame();

        void addPointLight(PointLight light, glm::mat4 transform);

        void addDirectionalLight(DirectionalLight light, glm::mat4 transform);

        void drawDynamicMesh(MeshData meshData);

        void drawStaticMesh(MeshData meshData);

        void destroyStaticMesh(unsigned int renderId);

        void clearColor(float r, float g, float b, float a);

        void setViewPortSize(int width, int height);

        unsigned int getDrawCallsCount();

        void setSkyBoxCubeMap(CubeMap* skyBoxCubeMap);

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
            Shader vertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/skyboxVertex.glsl" };
            Shader fragmentShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/skyboxFragment.glsl" };
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
            std::unique_ptr<VertexBuffer> vertexBuffer;
            std::unique_ptr<IndexBuffer> indexBuffer;
        };

        struct LightingData {
            std::vector<PointLight> pointLights;
            std::vector<DirectionalLight> directionalLights;

            UniformBuffer lightsUniformBuffer{ sizeof(LightsUniformBufferData) };
            UniformBuffer currentPointLightUniformBuffer{ sizeof(CurrentPointLightUniformBufferData) };
            UniformBuffer currentDirectionalLightUniformBuffer{ sizeof(CurrentDirectionalLightUniformBufferData) };

            ShaderProgram depthBufferTextureShaderProgram;
            Shader depthBufferTextureVertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/depthMapVertex.glsl" };
            Shader depthBufferTextureFragmentShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/depthMapFragment.glsl" };

            ShaderProgram depthBufferCubeMapShaderProgram;
            Shader depthBufferCubeMapVertexShader{ ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/cubeMapDepthMapVertex.glsl" };
            Shader depthBufferCubeMapGeometryShader{ ShaderType::GeometryShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/cubeMapDepthMapGeometry.glsl" };
            Shader depthBufferCubeMapFragmentShader{ ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/cubeMapDepthMapFragment.glsl" };
        };

        unsigned int drawCallsCount = 0;
        
        unsigned int currentTextureSlot = 0;
        unsigned int currentCubeMapSlot = 0;

        std::vector<Texture*> boundTextures;
        std::vector<Texture*> boundCubeMaps;

        DynamicRenderingData dynamicRenderingData;
        StaticRenderingData staticRenderingData;
        LightingData lightingData;
        SkyBoxData skyBoxData;

        Shader vertexShader { ShaderType::VertexShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/lightingVertex.glsl" };
        Shader fragmentShader { ShaderType::FragmentShader, std::string(ENGINE_ASSET_DIRECTORY) + "shaders/3d/lightingFragment.glsl" };
        ShaderProgram shaderProgram;

        UniformBuffer cameraUniformBuffer { sizeof(CameraUniformBufferData) };

        void updateLightsDepthBufferTextures();

        void drawStaticMeshes(ShaderProgram* targetShaderProgram, FrameBuffer* frameBufferTarget = nullptr);

        void drawDynamicMeshes(ShaderProgram* targetShaderProgram, FrameBuffer* frameBufferTarget = nullptr);

        void drawSkyBox();

        void flushDynamicRenderingBatch();

        void bindUniformBuffers();

        void bindTexture(Texture* texture);

        void bindCubeMap(Texture* texture);
                
        void updateCameraUniformBuffer(Camera& camera);

        void updateLightsUniformBuffers();

        void clearLights();

        void clearBoundTextures();

        void clearDynamicMeshes();

    };
}