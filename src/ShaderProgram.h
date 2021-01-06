#include "engine.h"
#include <iostream>

class ShaderProgram {

private: 
	unsigned int id;
public:

	ShaderProgram() {
		this->id = glCreateProgram();
	}

	void link() {
		glLinkProgram(this->id);
	}

	void use() {
		glUseProgram(this->id);

		int  success;
		char infoLog[512];

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	void attachShader(unsigned int shaderId) {
		glAttachShader(this->id, shaderId);
	}
};
