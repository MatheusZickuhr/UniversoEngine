#pragma once

#include <memory>
#include <iostream>

namespace engine {

	class DrawApi {

	public:

		static void init();

		static void initDebugMode();

		static void drawWithIdexes(uint32_t IndexCount);

		static void draw(uint32_t vertexCount);

		static void setViewPortSize(int32_t width, int32_t height);

		static int32_t getViewPortWidth();
		
		static int32_t getViewPortHeight();

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

