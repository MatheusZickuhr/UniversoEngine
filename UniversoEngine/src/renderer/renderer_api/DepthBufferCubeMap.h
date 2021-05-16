#pragma once
#include "Texture.h"

namespace engine {

	class DepthBufferCubeMap : public Texture {


	public:
		DepthBufferCubeMap(const float width, const float height);

		void bind(unsigned int slot);

		virtual void unbind() const ;

	};

}