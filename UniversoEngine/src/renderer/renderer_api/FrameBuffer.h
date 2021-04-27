#pragma once

#include "Texture.h"

namespace engine {

	class FrameBuffer {

	public:

		FrameBuffer();

		~FrameBuffer();

		void bind();

		void unbind();

		void addTextureAsDepthBuffer(Texture& texture);

	private:

		unsigned int id;
	};

}
