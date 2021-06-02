#pragma once

#include <memory>
#include <iostream>

namespace engine {

	class DrawApi {

	public:

		static void init();

		static void initDebugMode();

		static void drawWithIdexes(unsigned int IndexCount);

		static void draw(unsigned int vertexCount);

		static void setViewPortSize(float width, float height);

		static int getViewPortWidth();
		
		static int getViewPortHeight();

		static void clearColor(float r, float g, float b, float a);

		static void clearDepthBuffer();

		static void clearColorBuffer();

		static void clearDepthAndColorBuffer();

		static void cullFrontFace();

		static void cullBackFace();

		static void setDepthFunctionToLessOrEqual();

		static void setDepthFunctionToLess();

		static void enableDepthMask(bool enableDepthMask);

	};

}

