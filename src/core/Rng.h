#pragma once

#include <random>

class Rng {
public:
    Rng();

    unsigned int generate(unsigned int maxExclusive);

private:
    std::mt19937 m_rng;
};
