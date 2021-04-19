#pragma once

#include <memory>
#include <iostream>

namespace engine {

	class DrawApi {

	public:

		static void init();

		static void initDebugMode();

		static void drawWithIdexes(unsigned int IndexCount);

		static void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

		static void setViewPortSize(float width, float height);

	};

}

