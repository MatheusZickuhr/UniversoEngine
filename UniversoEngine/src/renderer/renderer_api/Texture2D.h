#pragma once
#include  "Texture.h"
#include <string>

namespace engine {
	
	class Texture2D : public Texture {

	public:

		Texture2D(const Texture2D& other) = delete;

		Texture2D(const std::string& filepath);

		void bind(unsigned int slot);

		void unbind() const;

	private:

		int bitsPerPixel;
	};
}