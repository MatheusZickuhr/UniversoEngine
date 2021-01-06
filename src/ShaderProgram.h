#include "engine.h"
#include <iostream>

class ShaderProgram {

private: 
	unsigned int id;
public:

	ShaderProgram() {
		this->id = glCreateProgram();
	}

	void unbind() {
		glUseProgram(0);
	}

	void bind() {
		glLinkProgram(this->id);

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

	void setUniform4f(const char* uniformName, float float1, float float2, float float3, float float4) {
		auto uniformLocation = glGetUniformLocation(this->id, uniformName);
		glUseProgram(this->id);
		glUniform4f(uniformLocation, float1, float2, float3, float4);
	}
};
