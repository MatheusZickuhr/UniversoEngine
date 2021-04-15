#include "Shader.h"
#include <glad/glad.h>
#include "../../debug/Assert.h"

namespace engine {

	Shader::Shader(ShaderType shaderType, const std::string& shaderFilePath) {

		ASSERT_FILE_EXISTS(shaderFilePath);

		ASSERT_FILE_EXTENSION(shaderFilePath, {".glsl"});

		auto shaderSource = this->readFile(shaderFilePath);
		auto shaderSourceCharPtr = shaderSource.c_str();

		//crete shader 
		switch (shaderType) {
		case ShaderType::FragmentShader:
			this->id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::VertexShader:
			this->id = glCreateShader(GL_VERTEX_SHADER);
			break;
		default:
			std::cout << "invalid shader type" << std::endl;
			break;
		}

		// compile
		glShaderSource(this->id, 1, &shaderSourceCharPtr, NULL);
		glCompileShader(this->id);

		// check for compile errors
		int  success;
		char infoLog[512];
		glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	}

	unsigned int Shader::getId() {
		return this->id;
	}

	void Shader::bind() {

	}
	void Shader::unbind() {

	}

	std::string Shader::readFile(std::string filePath) {
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