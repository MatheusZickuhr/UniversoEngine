#pragma once

#include "Texture.h"
#include "CubeMap.h"
#include <memory>

namespace engine {

	class FrameBuffer {

	public:

		FrameBuffer();

		FrameBuffer(const FrameBuffer& other) = delete;

		~FrameBuffer();

		void bind();

		void unbind();

		void addDepthTexture(std::shared_ptr<Texture> texture);

		void addDepthCubeMap(std::shared_ptr<CubeMap> cubeMap);
		
		static void bindDefaultFrameBuffer();

	private:

		unsigned int id;
	};

}
