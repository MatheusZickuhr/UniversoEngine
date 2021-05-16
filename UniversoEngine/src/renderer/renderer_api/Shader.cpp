#include "Shader.h"
#include <glad/glad.h>
#include "../../debug/Assert.h"
#include "../../utils/Format.h"

namespace engine {

	std::string readFile(const std::string& filePath);

	Shader::Shader(ShaderType shaderType, const std::string& filePath): shaderType(shaderType) {
		ASSERT_FILE_EXISTS(filePath);
		ASSERT_FILE_EXTENSION(filePath, {".glsl"});

		this->sourceCode = readFile(filePath);

		this->create();
	}

	void Shader::defineInt(const char* name, int value) {
		ASSERT(!this->compiled, "You must define before the shader compiles");

		this->intDefines.push_back({ name, value });
	}

	unsigned int Shader::getId() { return this->id; }

	void Shader::compile() {
		this->insertDefinesToSourceCode();

		const char* shaderSourceAsCharPtr = this->sourceCode.c_str();

		// compile
		glShaderSource(this->id, 1, &shaderSourceAsCharPtr, NULL);
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

	void Shader::insertDefinesToSourceCode() {
		auto indexOfVersion = this->sourceCode.find("version");
		auto insertIndex = this->sourceCode.find("\n", indexOfVersion) + 1;

		// insert defines of type int
		for (auto& define : this->intDefines) {
			this->sourceCode.insert(insertIndex, format("#define %s %d \n", define.name, define.value));
		}
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
}