#pragma once

#include <memory>
#include <iostream>

namespace engine {

	class DrawApi {

	public:

		static void init();

		static void initDebugMode();

		static void drawWithIdexes(unsigned int IndexCount);

		static void setViewPortSize(float width, float height);

		static void clearColor(float r, float g, float b, float a);

		static void clearDepthBuffer();

		static void clearColorBuffer();

		static void clearDepthAndColorBuffer();

		static void cullFrontFace();

		static void cullBackFace();

	};

}

