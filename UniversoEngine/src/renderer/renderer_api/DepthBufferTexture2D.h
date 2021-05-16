#pragma once
#include "Texture.h"

namespace engine {
	
	class DepthBufferTexture2D : public Texture {

	public:

		DepthBufferTexture2D(const DepthBufferTexture2D& other) = delete;

		DepthBufferTexture2D(const float width, const float height);

		void bind(unsigned int slot);

		void unbind() const;

	};
}