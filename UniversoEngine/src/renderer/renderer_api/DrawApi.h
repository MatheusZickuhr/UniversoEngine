#pragma once

#include <memory>
#include "VertexArray.h"
#include <iostream>

namespace engine {

	class DrawApi {

	public:

		DrawApi();

		void drawWithIdexes(VertexArray& vertexArray, unsigned int IndexCount);

		void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

		void setViewPortSize(float width, float height);

	};

}

