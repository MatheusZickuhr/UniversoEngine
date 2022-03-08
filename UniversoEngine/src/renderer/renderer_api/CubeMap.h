#pragma once
#include <string>
#include <vector>
#include <memory>

namespace engine {

	class CubeMap {

	public:

		const static uint32_t MAX_CUBEMAPS = 16;

		CubeMap();

		CubeMap(const CubeMap& other) = delete;

		~CubeMap();

		void bind(uint32_t slot);

		void unbind() const;

		uint32_t getId();

		int32_t getWidth();

		int32_t getHeight();

		uint32_t getSlot();

		static std::shared_ptr<CubeMap> createCubeMapFromFile(const std::vector<std::string>& filepaths);
		
		static std::shared_ptr<CubeMap> createDepthCubeMap(int32_t width, int32_t height);

	private:

		uint32_t id, slot = 0;
		int32_t width, height = 0;

	};
}
