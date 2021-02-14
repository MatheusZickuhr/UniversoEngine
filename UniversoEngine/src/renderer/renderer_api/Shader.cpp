#include "Shader.h"

namespace engine {

	Shader::Shader(ShaderType shaderType, std::string shaderPath) {
		auto shaderSource = this->readFile(shaderPath);
		auto shaderSourceCharPtr = shaderSource.c_str();

		//crete shader 
		switch (shaderType) {
		case FragmentShader:
			this->id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case VertexShader:
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