#pragma once

#include <iostream>
#include <glm/glm.hpp>

namespace engine {

	class ShaderProgram {

	private:
		unsigned int id;
	public:

		ShaderProgram();

		void unbind();

		void bind();

		void attachShader(unsigned int shaderId);

		void setUniform4f(const char* uniformName, float float1, float float2, float float3, float float4);

		void setUniformMat4f(const char* uniformName, const glm::mat4& matrix);

		void setUniform1f(const char* uniformName, const float value);

		void setUniform1i(const char* uniformName, const int value);

		void setUniform1iv(const char* uniformName, int size, int data[]);
	};
}
