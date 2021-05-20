#include "Shader.h"
#include <glad/glad.h>
#include "../../debug/Assert.h"
#include "../../utils/Format.h"

#include <filesystem>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_hlsl.hpp>
#include <sha256.h>

namespace engine {

	std::string readFile(const std::string& filePath);

	std::string shaderCodeToSha256(const std::string& shaderSourceCode);

	void cacheSpirvBinaryToFile(std::vector<uint32_t> spirvBinary, const std::string& filename);

	void createCacheFileHelper(const std::string& shaderSourceCode, const std::string& filename);

	std::vector<uint32_t> readSpirvBibaryFromFile(const std::string& filename);

	bool didShaderSourceChanged(const std::string& helperFilePath, const std::string& shaderSourceCode);

	Shader::Shader(ShaderType shaderType, const std::string& filePath): shaderType(shaderType) {
		ASSERT_FILE_EXISTS(filePath);
		ASSERT_FILE_EXTENSION(filePath, {".glsl"});

		this->sourceCode = readFile(filePath);

		this->fileName = std::filesystem::path(filePath).filename().string();

		this->create();
	}

	void Shader::addMacroDefinition(const std::string& name, const std::string& value) {
		ASSERT(!this->compiled, "You must define before the shader compiles");

		this->macroDefinitions.push_back({ name, value });
	}

	unsigned int Shader::getId() { return this->id; }

	void Shader::compile() {

		const std::string  cacheFolder = "shaderCache/";

		std::filesystem::create_directory(cacheFolder);

		const std::string cacheFilePath = cacheFolder + this->fileName + ".cache";
		const std::string cacheHelperFilePath = cacheFilePath + ".helper";

		std::vector<uint32_t> spirvBinary;

		if (didShaderSourceChanged(cacheHelperFilePath, this->sourceCode)) {
			std::cout << "compiling shader " << this->fileName << std::endl;
			spirvBinary = compileToSpirvBinary(this->fileName, this->sourceCode);
			cacheSpirvBinaryToFile(spirvBinary, cacheFilePath);
			createCacheFileHelper(this->sourceCode, cacheHelperFilePath);
			std::cout << "shader " << this->fileName << " compiled" << std::endl;
		}
		else {
			std::cout << "loading shader " << this->fileName << " from cache" << std::endl;
			spirvBinary = readSpirvBibaryFromFile(cacheFilePath);
			std::cout << "shader " << this->fileName << " loaded" << std::endl;
		}
		

		spirv_cross::CompilerGLSL glslCompiler(std::move(spirvBinary));

		spirv_cross::CompilerGLSL::Options options;
		options.version = 460;
		glslCompiler.set_common_options(options);

		std::string spirvGeneratedSource = glslCompiler.compile();
		const char* spirvGeneratedSourceAsCharPtr = spirvGeneratedSource.c_str();

		// compile
		glShaderSource(this->id, 1, &spirvGeneratedSourceAsCharPtr, NULL);
		glCompileShader(this->id);

		// check for compile errors
		int  success;
		char infoLog[512];
		glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}

		this->compiled = true;
	}

	void Shader::create() {
		//crete shader 
		switch (shaderType) {
		case ShaderType::FragmentShader:
			this->id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::VertexShader:
			this->id = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::GeometryShader:
			this->id = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		default:
			ASSERT(false, "invalid shader type");
			break;
		}
	}

	std::vector<uint32_t> Shader::compileToSpirvBinary(const std::string& sourceName, const std::string& source, bool optimize) {
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		
		for (auto& macroDef : this->macroDefinitions)
			options.AddMacroDefinition(macroDef.name, macroDef.value);

		if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

		shaderc_shader_kind shadercShaderKind = shaderc_glsl_fragment_shader;

		switch (shaderType) {
		case engine::ShaderType::FragmentShader:
			shadercShaderKind = shaderc_glsl_fragment_shader;
			break;
		case engine::ShaderType::VertexShader:
			shadercShaderKind = shaderc_glsl_vertex_shader;
			break;
		case engine::ShaderType::GeometryShader:
			shadercShaderKind = shaderc_glsl_geometry_shader;
			break;
		default:
			ASSERT(false, "Invalid shader type");
			break;
		}

		shaderc::SpvCompilationResult module =
			compiler.CompileGlslToSpv(source, shadercShaderKind, sourceName.c_str(), options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			std::cerr << module.GetErrorMessage();
			return std::vector<uint32_t>();
		}

		return { module.cbegin(), module.cend() };
	}

	std::string readFile(const std::string& filePath) {
		std::ifstream file;
		std::string line;
		std::string shaderSource;

		file.open(filePath);

		while (std::getline(file, line)) {
			shaderSource.append(line);
			shaderSource.append("\n");
		}

		return shaderSource;
	}

	std::string shaderCodeToSha256(const std::string& shaderSourceCode) {
		SHA256 sha256;
		return sha256(shaderSourceCode);
	}

	void cacheSpirvBinaryToFile(std::vector<uint32_t> spirvBinary, const std::string& filename) {
		// create cache file
		std::fstream cacheFile;
		cacheFile.open(filename, std::ios::out | std::ios::binary);
		cacheFile.write((char*)spirvBinary.data(), spirvBinary.size() * sizeof(uint32_t));
		cacheFile.flush();
		cacheFile.close();
	}

	void createCacheFileHelper(const std::string& shaderSourceCode, const std::string& filename) {
		// create helper file
		std::ofstream infoFile;
		infoFile.open(filename);
		infoFile << shaderCodeToSha256(shaderSourceCode);
		infoFile.flush();
		infoFile.close();
	}

	std::vector<uint32_t> readSpirvBibaryFromFile(const std::string& filename) {
		std::fstream file;
		file.open(filename, std::ios::in | std::ios::binary);

		file.seekg(0, std::ios::end);
		auto fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint32_t> result;
		result.resize(fileSize / sizeof(uint32_t));


		file.read((char*)result.data(), fileSize);

		return result;
	}
	/* read the helper file to determinate if the shader source code changed based on a sha256 hash */
	bool didShaderSourceChanged(const std::string& helperFilePath, const std::string& shaderSourceCode) {
		if (!std::filesystem::exists(helperFilePath)) return true;

		std::ifstream helperFile(helperFilePath);

		std::string fileSha256;
		std::getline(helperFile, fileSha256);

		const std::string shaderSourceSha256 = shaderCodeToSha256(shaderSourceCode);

		return fileSha256 != shaderSourceSha256;
	}
}