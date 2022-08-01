#include <catch2/catch_all.hpp>
#include "core/Rng.h"

#define RNG_TAG "[Rng]"

TEST_CASE("always in range", RNG_TAG) {
    Rng rng;
    for (int i = 0; i < 100; i++) {
        auto result = rng.generate(1000);
        CHECK((result >= 0 && result < 1000));
    }
}

TEST_CASE("returns 0 if range too small", RNG_TAG) {
    Rng rng;
    CHECK(rng.generate(0) == 0);
    CHECK(rng.generate(1) == 0);
}
