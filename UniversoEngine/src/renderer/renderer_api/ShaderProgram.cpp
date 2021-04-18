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

	void ShaderProgram::setUniform3f(const std::string& uniformName, float float1, float float2, float float3) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform3f(uniformLocation, float1, float2, float3);
	}

	void ShaderProgram::setUniform4f(const std::string& uniformName, float float1, float float2, float float3, float float4) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform4f(uniformLocation, float1, float2, float3, float4);
	}

	void ShaderProgram::setUniformMat4f(const std::string& uniformName, const glm::mat4& matrix) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]);

	}

	void ShaderProgram::setUniform1f(const std::string& uniformName, const float value) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform1f(uniformLocation, value);
	}

	void ShaderProgram::setUniform1i(const std::string& uniformName, const int value) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform1i(uniformLocation, value);
	}

	void ShaderProgram::setUniform1iv(const std::string& uniformName, int size, int data[]) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform1iv(uniformLocation, size, data);
	}

}