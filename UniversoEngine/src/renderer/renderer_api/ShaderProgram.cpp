#include "ShaderProgram.h"
#include <glad/glad.h>

namespace engine {

	ShaderProgram::ShaderProgram() {
		this->id = glCreateProgram();
	}

	void ShaderProgram::unbind() {
		glUseProgram(0);
	}

	void ShaderProgram::bind() {
		if (!this->linked) {
			this->linked = true;
			
			glLinkProgram(this->id);
			
			int  success;
			char infoLog[512];

			glGetProgramiv(this->id, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(this->id, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		glUseProgram(this->id);

	}

	void ShaderProgram::attachShader(unsigned int shaderId) {
		glAttachShader(this->id, shaderId);
	}

	void ShaderProgram::setUniform3f(const char* uniformName, float float1, float float2, float float3) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName);
		glUniform3f(uniformLocation, float1, float2, float3);
	}

	void ShaderProgram::setUniform4f(const char* uniformName, float float1, float float2, float float3, float float4) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName);
		glUniform4f(uniformLocation, float1, float2, float3, float4);
	}

	void ShaderProgram::setUniformMat4f(const char* uniformName, const glm::mat4& matrix) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]);

	}

	void ShaderProgram::setUniform1f(const char* uniformName, const float value) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName);
		glUniform1f(uniformLocation, value);
	}

	void ShaderProgram::setUniform1i(const char* uniformName, const int value) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName);
		glUniform1i(uniformLocation, value);
	}

	void ShaderProgram::setUniform1iv(const char* uniformName, int size, int data[]) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName);
		glUniform1iv(uniformLocation, size, data);
	}

}