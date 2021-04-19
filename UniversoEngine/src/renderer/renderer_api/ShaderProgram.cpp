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

	void ShaderProgram::setVec3Uniform(const std::string& uniformName, const glm::vec3& vec) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform3f(uniformLocation, vec.x, vec.y, vec.z);
	}

	void ShaderProgram::setVec4Uniform(const std::string& uniformName, const glm::vec4& vec) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform4f(uniformLocation, vec.x, vec.y, vec.z, vec.w);
	}

	void ShaderProgram::setMat4Uniform(const std::string& uniformName, const glm::mat4& matrix) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]);

	}

	void ShaderProgram::setFloatUniform(const std::string& uniformName, const float value) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform1f(uniformLocation, value);
	}

	void ShaderProgram::setIntUniform(const std::string& uniformName, const int value) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform1i(uniformLocation, value);
	}

	void ShaderProgram::setIntArrayUniform(const std::string& uniformName, int size, int data[]) {
		this->bind();
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		glUniform1iv(uniformLocation, size, data);
	}

}