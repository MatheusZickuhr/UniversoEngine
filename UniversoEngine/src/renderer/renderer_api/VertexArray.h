#pragma once

#include <glad/glad.h>

namespace engine {
	
	class VertexArray {

	private:
		unsigned int id;

	public:
		VertexArray();

		~VertexArray();

		void bind();

		void unbind();
	};
}