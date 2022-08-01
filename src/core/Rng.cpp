#include "Rng.h"

unsigned int Rng::generate(unsigned int maxExclusive) {
    if (maxExclusive <= 1) return 0;
    else {
        return m_rng() % maxExclusive;
    }
}

std::mt19937 createAndInitialize() {
    std::random_device rd;
    std::seed_seq sd{rd(), rd(), rd(), rd()};
    return std::mt19937(sd);
}

Rng::Rng() : m_rng(createAndInitialize()) {};
