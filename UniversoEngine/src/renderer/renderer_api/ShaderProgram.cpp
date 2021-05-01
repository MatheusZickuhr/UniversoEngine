#include "ShaderProgram.h"
#include <glad/glad.h>

#include "../../debug/Assert.h"

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
		glUniform3f(this->findUniformLocation(uniformName), vec.x, vec.y, vec.z);
	}

	void ShaderProgram::setVec4Uniform(const std::string& uniformName, const glm::vec4& vec) {
		this->bind();
		glUniform4f(this->findUniformLocation(uniformName), vec.x, vec.y, vec.z, vec.w);
	}

	void ShaderProgram::setMat4Uniform(const std::string& uniformName, const glm::mat4& matrix) {
		this->bind();
		glUniformMatrix4fv(this->findUniformLocation(uniformName), 1, GL_FALSE, &matrix[0][0]);

	}

	void ShaderProgram::setFloatUniform(const std::string& uniformName, const float value) {
		this->bind();
		glUniform1f(this->findUniformLocation(uniformName), value);
	}

	void ShaderProgram::setIntUniform(const std::string& uniformName, const int value) {
		this->bind();
		glUniform1i(this->findUniformLocation(uniformName), value);
	}

	void ShaderProgram::setIntArrayUniform(const std::string& uniformName, int size, int data[]) {
		this->bind();
		glUniform1iv(this->findUniformLocation(uniformName), size, data);
	}

	int ShaderProgram::findUniformLocation(const std::string& uniformName) {
		auto uniformLocation = glGetUniformLocation(this->id, uniformName.c_str());
		ASSERT(uniformLocation != -1, "Cannot find uniform `" + uniformName + "`");
		return uniformLocation;
	}

}