#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <string>
namespace engine {

	class ShaderProgram {

	public:

		ShaderProgram();

		void unbind();

		void bind();

		void attachShader(unsigned int shaderId);

		void setUniform3f(const std::string& uniformName, float float1, float float2, float float3);

		void setUniform4f(const std::string& uniformName, float float1, float float2, float float3, float float4);

		void setUniformMat4f(const std::string& uniformName, const glm::mat4& matrix);

		void setUniform1f(const std::string& uniformName, const float value);

		void setUniform1i(const std::string& uniformName, const int value);

		void setUniform1iv(const std::string& uniformName, int size, int data[]);

	private:

		unsigned int id;
		bool linked = false;
	};
}
