#pragma once

#include <memory>
#include <glad/glad.h>
#include "VertexArray.h"

namespace engine {

	void APIENTRY GLDebugMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* msg,
		const void* data
	);

	class Drawer {

	public:

		Drawer();

		void drawWithIdexes(std::shared_ptr<VertexArray> vertexArray, unsigned int IndexCount);

		void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	};

}

