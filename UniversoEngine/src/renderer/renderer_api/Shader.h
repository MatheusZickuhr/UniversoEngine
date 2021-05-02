#pragma once
#include <iostream>
#include <fstream>
#include <string>

namespace engine {

	enum class ShaderType { FragmentShader, VertexShader, GeometryShader };

	class Shader {

	public:

		Shader(ShaderType shaderType, const std::string& shaderFilePath);

		unsigned int getId();

		void bind();

		void unbind();

	private:

		unsigned int id;

		std::string readFile(std::string filePath);
	};
}