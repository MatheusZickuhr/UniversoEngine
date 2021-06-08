#pragma once

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

        struct DrawData {
            Mesh* mesh;
            Material* material;
            glm::mat4 transform;
        };

        static Renderer3D& getInstance() {
            static Renderer3D renderer3d;
            return renderer3d;
        }

        Renderer3D(Renderer3D const& other) = delete;

        ~Renderer3D();

        void beginFrame(Camera& camera);

        void endFrame();

        void addPointLight(PointLight light, glm::mat4 transform);

        void addDirectionalLight(DirectionalLight light, glm::mat4 transform);

        void addDrawData(DrawData drawData) { frameDrawData.push_back(drawData); }

        void clearColor(float r, float g, float b, float a) { DrawApi::clearColor(r, g, b, a); }

        void setViewPortSize(int width, int height) { DrawApi::setViewPortSize(width, height); }

        unsigned int getDrawCallsCount() { return this->drawCallsCount; }

        void setSkyBoxCubeMap(CubeMap* skyBoxCubeMap) { this->skyBoxCubeMap = skyBoxCubeMap; }

    private:

        struct CameraUniformBufferData {
            glm::mat4 cameraViewProjectionMatrix;
            glm::mat4 cameraViewMatrix;
            glm::mat4 cameraProjectionMatrix;
            glm::vec3 cameraPosition;
        };

        struct LightsUniformBufferData {
            PointLight::Data pointLights[PointLight::maxPointLights];
            DirectionalLight::Data directionalLights[DirectionalLight::maxDirectionalLights];
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

        // arbitrary values for now
        const unsigned int maxVertices = 10000;
        const unsigned int maxIndices  = 10000;

        unsigned int drawCallsCount = 0;

        std::vector<DrawData> frameDrawData;

        std::vector<Texture*> boundTextures;
        unsigned int currentTextureSlot = 0;

        std::vector<Texture*> boundCubeMaps;
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

        // skybox cubemap
        CubeMap* skyBoxCubeMap = nullptr;

        VertexArray skyBoxVertexArray;
        VertexBuffer skyBoxVertexBuffer{ sizeof(glm::vec3), 36 };

        ShaderProgram skyBoxShaderProgram;
        Shader skyBoxVertexShader{ ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/skyboxVertex.glsl" };
        Shader skyBoxFragmentShader{ ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/skyboxFragment.glsl" };

        Renderer3D();

        void renderPass();

        void shadowPass();

        void bindUniformBuffers();

        void bindTexture(Texture* texture);

        void bindCubeMap(Texture* texture);
                
        void performDrawCall();

        void performShadowMapDrawCalls();

    };
}