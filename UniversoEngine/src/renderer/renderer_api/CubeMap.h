#pragma once
#include <string>
#include <vector>
#include <memory>

namespace engine {

	class CubeMap {

	public:

		const static unsigned int MAX_CUBEMAPS = 16;

		CubeMap();

		CubeMap(const CubeMap& other) = delete;

		~CubeMap();

		void bind(unsigned int slot);

		void unbind() const;

		unsigned int getId();

		int getWidth();

		int getHeight();

		unsigned int getSlot();

		static std::shared_ptr<CubeMap> createCubeMapFromFile(const std::vector<std::string>& filepaths);
		
		static std::shared_ptr<CubeMap> createDepthCubeMap(int width, int height);

	private:

		unsigned int id, slot = 0;
		int width, height = 0;

	};
}
