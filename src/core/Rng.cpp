#include "Rng.h"

unsigned int Rng::generate(unsigned int maxExclusive) {
    if (maxExclusive <= 1) return 0;
    else {
        return m_rng() % maxExclusive;
    }
}
