#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>

namespace engine {

	enum ShaderType { FragmentShader, VertexShader };

	class Shader {

	private:
		unsigned int id;

	public:

		Shader(ShaderType shaderType, std::string shaderPath);

		unsigned int getId();

		void bind();

		void unbind();

	private:

		std::string readFile(std::string filePath);
	};
}