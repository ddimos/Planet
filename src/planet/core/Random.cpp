#include "Random.h"

void Random::init(unsigned _seed)
{
    m_generator.seed(_seed);
}

int Random::rand(int _min, int _max)
{
    double requiredRange = _max - _min;
    double generatorRange = m_generator.max() - m_generator.min();
    double scale = requiredRange / (generatorRange + 1.0);
    return (m_generator() - m_generator.min()) * scale + _min;
}
