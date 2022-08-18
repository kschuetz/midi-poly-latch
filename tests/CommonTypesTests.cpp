#include <catch2/catch_all.hpp>
#include "core/CommonTypes.h"

#define NOTE_NUMBER_TAG "[NoteNumber]"

TEST_CASE("isValidNote", NOTE_NUMBER_TAG) {
    CHECK(isValidNote(0));
    CHECK(isValidNote(1));
    CHECK(isValidNote(127));
    CHECK_FALSE(isValidNote(-1));
    CHECK_FALSE(isValidNote(-2));
    CHECK_FALSE(isValidNote(-128));
}

#define VELOCITY_TAG

TEST_CASE("constructing Velocity", VELOCITY_TAG) {
    CHECK(Velocity(0) == Velocity(0));
    CHECK(Velocity(1) == Velocity(1));
    CHECK(Velocity(127) == Velocity(127));
    CHECK(Velocity(-1) == Velocity(127));
    CHECK(Velocity(-2) == Velocity(126));
    CHECK(Velocity(128) == Velocity(0));
    CHECK(Velocity(-128) == Velocity(0));
}
