#pragma once

#include <random>
#include <chrono>

class Rng {
public:
    int generate(int maxExclusive);

    Rng() : m_rng(std::chrono::system_clock::now().time_since_epoch().count()) {}

private:
    std::mt19937 m_rng;
};
