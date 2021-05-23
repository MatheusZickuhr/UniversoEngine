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

		unsigned int getId() { return this->id; }

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
		std::string cacheFilePath;
		std::string cacheHelperFilePath;
		std::vector<MacroDefinition> macroDefinitions;
		
		std::vector<uint32_t> compileToSpirvBinary(bool optimize = false);

		void cacheSpirvBinaryToFile(std::vector<uint32_t> spirvBinary);

		std::vector<uint32_t> readSpirvBinaryFromFile();

		std::string readSourceCodeFromFile(const std::string& filePath);

		bool isSpirvBinaryCacheFileInvalid();

		void createOpenglShader();

	};
}