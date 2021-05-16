#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace engine {

	enum class ShaderType { FragmentShader, VertexShader, GeometryShader };

	struct IntDefine {
		const char* name;
		int value;
	};

	class Shader {

	public:

		Shader(ShaderType shaderType, const std::string& shaderFilePath);

		Shader(const Shader& other) = delete;

		unsigned int getId();

		void defineInt(const char* name, int value);
		
		void compile();

	private:

		unsigned int id;
		bool compiled = false;
		ShaderType shaderType;
		std::string sourceCode;
		std::vector<IntDefine> intDefines;

		void insertDefinesToSourceCode();

		void create();

	};
}