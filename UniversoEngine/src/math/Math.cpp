#include "Math.h"
#include "PerlinNoise.hpp"

namespace engine {
	
	static siv::PerlinNoise perlin;
	
	double Math::perlinNoise2D(double x, double y) {
		
		return perlin.noise2D(x, y);
	}

	void Math::reseedPerlinNoise(std::uint32_t seed) {
		perlin.reseed(seed);
	}

	float Math::map(float value, float rangeStart1, float rangeEnd1, float rangeStart2, float rangeEnd2) {
		return rangeStart2 + (rangeEnd2 - rangeStart2) * ((value - rangeStart1) / (rangeEnd1 - rangeStart1));
	}

}