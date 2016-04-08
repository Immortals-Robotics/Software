#include "Random.h"

Random::Random() : Random(float(0), float(1))
{
}

Random::Random(float _min, float _max)
{
	min = _min;
	max = _max;

	rnd_device = new std::random_device();
	rnd_engine = new std::mt19937((*rnd_device)());
	rnd_dist = new std::uniform_real_distribution<float>(min, max);
}

Random::~Random()
{
	delete rnd_device;
	delete rnd_engine;
	delete rnd_dist;
}

float Random::get() const
{
	return (*rnd_dist)(*rnd_engine);
}
