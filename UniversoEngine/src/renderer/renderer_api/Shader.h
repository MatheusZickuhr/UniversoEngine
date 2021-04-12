#pragma once
#include <iostream>
#include <fstream>
#include <string>

namespace engine {

	enum ShaderType { FragmentShader, VertexShader };

	class Shader {

	private:
		unsigned int id;

	public:

		Shader(ShaderType shaderType, const std::string& shaderFilePath);

		unsigned int getId();

		void bind();

		void unbind();

	private:

		std::string readFile(std::string filePath);
	};
}