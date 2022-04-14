#pragma once
#include <cstdint>

namespace engine {
	
	class Math {

	public:

		const static float PI;

		static double perlinNoise2D(double x, double y);

		static void reseedPerlinNoise(std::uint32_t seed);

		static float map(float value, float rangeStart1, float rangeEnd1, float rangeStart2, float rangeEnd2);
	};

}