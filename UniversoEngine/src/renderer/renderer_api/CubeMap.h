#pragma once
#include "Texture.h"
#include <string>
#include <vector>

namespace engine {
	class CubeMap : public Texture {

	public:

		CubeMap(const CubeMap& other) = delete;

		CubeMap(const std::vector<std::string>& filepaths);

		void bind(unsigned int slot);

		void unbind() const;
	};
}
