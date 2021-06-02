#pragma once

#include <vector>
#include <string>
#include <memory>
#include "renderer_api/CubeMap.h"
#include "renderer_api/VertexArray.h"
#include "renderer_api/VertexBuffer.h"
#include "renderer_api/ShaderProgram.h"
#include "renderer_api/Shader.h"
#include "Mesh.h"

namespace engine {

    class Renderer3D;

	class CubeMapSkyBox {

	public:

		CubeMapSkyBox(std::vector<std::string> facesFilesPaths);

	private:

        friend class Renderer3D;

        std::unique_ptr<CubeMap> cubeMap = nullptr;

        VertexArray vertexArray;
        VertexBuffer vertexBuffer { sizeof(glm::vec3), 36 };

        ShaderProgram shaderProgram;
        Shader vertexShader{ ShaderType::VertexShader, "UniversoEngine/resources/shaders/3d/skyboxVertex.glsl" };
        Shader fragShader{ ShaderType::FragmentShader, "UniversoEngine/resources/shaders/3d/skyboxFragment.glsl" };
	};

}