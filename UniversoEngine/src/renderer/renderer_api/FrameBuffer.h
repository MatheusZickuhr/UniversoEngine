#pragma once

#include "DepthBufferTexture2D.h"

namespace engine {

	class FrameBuffer {

	public:

		FrameBuffer();

		~FrameBuffer();

		void bind();

		void unbind();

		void addDepthBufferTexture(DepthBufferTexture2D& texture);

	private:

		unsigned int id;
	};

}
