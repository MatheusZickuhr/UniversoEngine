#include "Shader.h"
#include <glad/glad.h>
#include "../../debug/Assert.h"
#include "../../debug/Log.h"

#include <format>
#include <filesystem>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_hlsl.hpp>
#include <sha256.h>

namespace engine {

	Shader::Shader(ShaderType shaderType, const std::string& filePath): shaderType(shaderType) {
		ASSERT_FILE_EXISTS(filePath);
		ASSERT_FILE_EXTENSION(filePath, {".glsl"});

		this->sourceCode = readSourceCodeFromFile(filePath);

		this->fileName = std::filesystem::path(filePath).filename().string();

		const std::string parentPath = std::filesystem::path(filePath).parent_path().string();

		const std::string  cacheFolder = "shaderCache/" + parentPath + "/";

		std::filesystem::create_directories(cacheFolder);

		this->cacheFilePath = cacheFolder + this->fileName + ".cache";
		this->cacheHelperFilePath = cacheFilePath + ".helper";

		this->createOpenglShader();
	}

	void Shader::addMacroDefinition(const std::string& name, const std::string& value) {
		ASSERT(!this->compiled, "You must define before the shader compiles");

		this->macroDefinitions.push_back({ name, value });
	}

	void Shader::compile() {
		// load the spirv binary from a cache file or directly from the source code by compiling
		std::vector<uint32_t> spirvBinary;

		if (this->isSpirvBinaryCacheFileInvalid()) {
			LOG(std::format("compiling shader {}", this->fileName));
			spirvBinary = this->compileToSpirvBinary();
			this->cacheSpirvBinaryToFile(spirvBinary);
		}
		else {
			LOG(std::format("loading shader {} from cache", this->fileName));
			spirvBinary = this->readSpirvBinaryFromFile();
		}
		
		// compiles the spirv binary to glsl opengl compatible code
		spirv_cross::CompilerGLSL glslCompiler(std::move(spirvBinary));

		spirv_cross::CompilerGLSL::Options options;
		options.version = 440;
		glslCompiler.set_common_options(options);

		std::string spirvGeneratedSource = glslCompiler.compile();
		const char* spirvGeneratedSourceAsCharPtr = spirvGeneratedSource.c_str();

		// give the generated spirv glsl code to opengl
		glShaderSource(this->id, 1, &spirvGeneratedSourceAsCharPtr, NULL);
		
		// let opengl compile the source code
		glCompileShader(this->id);

		// check for compile errors
		int32_t  success;
		char infoLog[512];
		glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(this->id, 512, NULL, infoLog);
			LOG(std::format("ERROR::SHADER::COMPILATION_FAILED\n{}", infoLog));
			return;
		}

		this->compiled = success;
	}

	void Shader::createOpenglShader() {
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

	std::vector<uint32_t> Shader::compileToSpirvBinary(bool optimize) {
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
			compiler.CompileGlslToSpv(this->sourceCode, shadercShaderKind, this->fileName.c_str(), options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			LOG(module.GetErrorMessage());
			return std::vector<uint32_t>();
		}

		return { module.cbegin(), module.cend() };
	}

	std::string Shader::readSourceCodeFromFile(const std::string& filePath) {
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

	void Shader::cacheSpirvBinaryToFile(std::vector<uint32_t> spirvBinary) {
		// create cache file
		std::fstream cacheFile;
		cacheFile.open(this->cacheFilePath, std::ios::out | std::ios::binary);
		cacheFile.write((char*)spirvBinary.data(), spirvBinary.size() * sizeof(uint32_t));
		cacheFile.flush();
		cacheFile.close();

		// create helper file
		SHA256 sha256;

		std::ofstream infoFile;
		infoFile.open(this->cacheHelperFilePath);
		infoFile << sha256(this->sourceCode);
		infoFile.flush();
		infoFile.close();
	}

	std::vector<uint32_t> Shader::readSpirvBinaryFromFile() {
		std::fstream file;
		file.open(this->cacheFilePath, std::ios::in | std::ios::binary);

		file.seekg(0, std::ios::end);
		auto fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint32_t> result;
		result.resize(fileSize / sizeof(uint32_t));


		file.read((char*)result.data(), fileSize);

		return result;
	}

	/* read the helper file to determinate if the shader source code changed based on a sha256 hash */
	bool Shader::isSpirvBinaryCacheFileInvalid() {
		if (!std::filesystem::exists(this->cacheFilePath)) return true;

		if (!std::filesystem::exists(this->cacheHelperFilePath)) return true;

		std::ifstream helperFile(this->cacheHelperFilePath);

		std::string oldSourceCodeSha256;
		std::getline(helperFile, oldSourceCodeSha256);

		SHA256 sha256;

		return oldSourceCodeSha256 != sha256(this->sourceCode);
	}
}