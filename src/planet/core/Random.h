#pragma once
#include <random>

class Random
{
public:
    Random() = default;
    ~Random() = default;

    void init(unsigned _seed);
    int rand(int _min, int _max);    // [_min, _max) _max is not included

private:
    std::mt19937 m_generator;
};
