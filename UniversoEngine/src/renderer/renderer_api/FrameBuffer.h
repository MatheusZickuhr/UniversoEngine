#pragma once

#include "DepthBufferTexture2D.h"
#include "DepthBufferCubeMap.h"

namespace engine {

	class FrameBuffer {

	public:

		FrameBuffer();

		~FrameBuffer();

		void bind();

		void unbind();

		void addDepthBufferTexture(DepthBufferTexture2D& texture);

		void addDepthBufferCubeMap(DepthBufferCubeMap& cubeMap);

	private:

		unsigned int id;
	};

}
