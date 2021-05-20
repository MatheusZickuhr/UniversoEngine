#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace engine {

	enum class ShaderType { FragmentShader, VertexShader, GeometryShader };



	class Shader {


	public:

		Shader(ShaderType shaderType, const std::string& shaderFilePath);

		Shader(const Shader& other) = delete;

		unsigned int getId();

		void addMacroDefinition(const std::string& name, const std::string& value);
		
		void compile();

	private:

		struct MacroDefinition {
			const std::string name;
			const std::string value;
		};

		unsigned int id;
		bool compiled = false;
		ShaderType shaderType;
		std::string sourceCode;
		std::string fileName;
		std::vector<MacroDefinition> macroDefinitions;
		
		std::vector<uint32_t> compileToSpirvBinary(const std::string& sourceName, const std::string& source, bool optimize = false);

		void create();

	};
}