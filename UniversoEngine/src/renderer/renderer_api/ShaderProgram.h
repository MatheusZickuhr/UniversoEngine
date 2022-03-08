#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <string>

#include "Shader.h"

namespace engine {

	class ShaderProgram {

	public:

		ShaderProgram();

		ShaderProgram(const ShaderProgram& other) = delete;

		void unbind();

		void bind();

		void attachShader(Shader& shader);

		void setVec3Uniform(const std::string& uniformName, const glm::vec3& vec);

		void setVec4Uniform(const std::string& uniformName, const glm::vec4& vec);

		void setMat4Uniform(const std::string& uniformName, const glm::mat4& matrix);

		void setFloatUniform(const std::string& uniformName, const float value);

		void setIntUniform(const std::string& uniformName, const int32_t value);

		void setIntArrayUniform(const std::string& uniformName, int32_t size, int32_t data[]);

	private:

		uint32_t id;
		bool linked = false;

		int32_t findUniformLocation(const std::string& uniformName);
	};
}
