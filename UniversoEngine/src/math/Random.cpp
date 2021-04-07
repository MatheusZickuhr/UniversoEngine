#include "Random.h"
#include <cstdlib>
#include <ctime>

Random::Random() {
	std::srand((unsigned)time(NULL));
}

float Random::random() {
	return  (float)rand() / RAND_MAX;
}
