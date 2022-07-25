#include <catch2/catch_all.hpp>
#include "core/NoteIndex.h"

#define TEST_GROUP "[NoteIndex]"

TEST_CASE("inserts notes in the proper position", TEST_GROUP) {
    NoteIndex ni;
    REQUIRE(ni.insert(0) == 0);
    REQUIRE(ni.positionOf(0) == 0);
    REQUIRE(ni.contains(0));
    REQUIRE(ni.size() == 1);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 0);

    REQUIRE(ni.insert(1) == 1);
    REQUIRE(ni.positionOf(1) == 1);
    REQUIRE(ni.contains(1));
    REQUIRE(ni.size() == 2);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 1);

    REQUIRE(ni.insert(0) == 0);
    REQUIRE(ni.positionOf(0) == 0);
    REQUIRE(ni.contains(0));
    REQUIRE(ni.size() == 2);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 1);

    REQUIRE(ni.insert(127) == 2);
    REQUIRE(ni.positionOf(127) == 2);
    REQUIRE(ni.contains(127));
    REQUIRE(ni.size() == 3);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 127);

    REQUIRE(ni.insert(64) == 2);
    REQUIRE(ni.positionOf(64) == 2);
    REQUIRE(ni.contains(64));
    REQUIRE(ni.size() == 4);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 127);

    REQUIRE(ni.insert(65) == 3);
    REQUIRE(ni.positionOf(65) == 3);
    REQUIRE(ni.contains(65));
    REQUIRE(ni.size() == 5);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 127);

    REQUIRE(ni.insert(96) == 4);
    REQUIRE(ni.positionOf(96) == 4);
    REQUIRE(ni.contains(96));
    REQUIRE(ni.size() == 6);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 127);

    REQUIRE(ni.insert(2) == 2);
    REQUIRE(ni.positionOf(2) == 2);
    REQUIRE(ni.contains(2));
    REQUIRE(ni.size() == 7);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 127);
}

TEST_CASE("iterates correctly", TEST_GROUP) {
    NoteIndex ni;
    ni.insert(0);
    ni.insert(1);
    ni.insert(2);
    ni.insert(64);
    ni.insert(65);
    ni.insert(96);
    ni.insert(127);

    SECTION("forward") {
        auto it = ni.cbegin();
        REQUIRE(*it == 0);
        REQUIRE(*(++it) == 1);
        REQUIRE(*(++it) == 2);
        REQUIRE(*(++it) == 64);
        REQUIRE(*(++it) == 65);
        REQUIRE(*(++it) == 96);
        REQUIRE(*(++it) == 127);
        REQUIRE(++it == ni.cend());
    }

    SECTION("reverse") {
        auto it = ni.crbegin();
        REQUIRE(*it == 127);
        REQUIRE(*(++it) == 96);
        REQUIRE(*(++it) == 65);
        REQUIRE(*(++it) == 64);
        REQUIRE(*(++it) == 2);
        REQUIRE(*(++it) == 1);
        REQUIRE(*(++it) == 0);
        REQUIRE(++it == ni.crend());
    }
}

TEST_CASE("empty", TEST_GROUP) {
    NoteIndex ni;

    REQUIRE(ni.size() == 0);
    REQUIRE(ni.front() == NO_NOTE);
    REQUIRE(ni.back() == NO_NOTE);
    REQUIRE(ni.cbegin() == ni.cend());
    REQUIRE(ni.crbegin() == ni.crend());

    SECTION("contains no notes") {
        int containsCount = 0;
        for (NoteNumber n = 0; isValidNote(n); n++) {
            if (ni.contains(n)) containsCount++;
        }
        REQUIRE(containsCount == 0);
    }
}

TEST_CASE("insert does not accept illegal notes", TEST_GROUP) {
    NoteIndex ni;
    REQUIRE(ni.insert(NO_NOTE) == -1);
    REQUIRE(ni.insert(NO_NOTE - 1) == -1);
}

TEST_CASE("removes notes", TEST_GROUP) {
    NoteIndex ni;

    ni.insert(0);
    ni.insert(1);
    ni.insert(2);
    ni.insert(64);
    ni.insert(65);
    ni.insert(96);
    ni.insert(127);

    SECTION("remove non-existing note") {
        REQUIRE(ni.remove(126) == false);
        REQUIRE(ni.size() == 7);

        REQUIRE(ni.front() == 0);
        REQUIRE(ni.back() == 127);

        auto it1 = ni.cbegin();
        REQUIRE(*it1 == 0);
        REQUIRE(*(++it1) == 1);
        REQUIRE(*(++it1) == 2);
        REQUIRE(*(++it1) == 64);
        REQUIRE(*(++it1) == 65);
        REQUIRE(*(++it1) == 96);
        REQUIRE(*(++it1) == 127);
        REQUIRE(++it1 == ni.cend());

        auto it2 = ni.crbegin();
        REQUIRE(*it2 == 127);
        REQUIRE(*(++it2) == 96);
        REQUIRE(*(++it2) == 65);
        REQUIRE(*(++it2) == 64);
        REQUIRE(*(++it2) == 2);
        REQUIRE(*(++it2) == 1);
        REQUIRE(*(++it2) == 0);
        REQUIRE(++it2 == ni.crend());
    }

    SECTION("remove existing note from middle") {
        REQUIRE(ni.remove(64) == true);
        REQUIRE(ni.size() == 6);

        REQUIRE(ni.front() == 0);
        REQUIRE(ni.back() == 127);

        auto it1 = ni.cbegin();
        REQUIRE(*it1 == 0);
        REQUIRE(*(++it1) == 1);
        REQUIRE(*(++it1) == 2);
        REQUIRE(*(++it1) == 65);
        REQUIRE(*(++it1) == 96);
        REQUIRE(*(++it1) == 127);
        REQUIRE(++it1 == ni.cend());

        auto it2 = ni.crbegin();
        REQUIRE(*it2 == 127);
        REQUIRE(*(++it2) == 96);
        REQUIRE(*(++it2) == 65);
        REQUIRE(*(++it2) == 2);
        REQUIRE(*(++it2) == 1);
        REQUIRE(*(++it2) == 0);
        REQUIRE(++it2 == ni.crend());
    }

    SECTION("remove existing note from front") {
        REQUIRE(ni.remove(0) == true);
        REQUIRE(ni.size() == 6);

        REQUIRE(ni.front() == 1);
        REQUIRE(ni.back() == 127);

        auto it1 = ni.cbegin();
        REQUIRE(*it1 == 1);
        REQUIRE(*(++it1) == 2);
        REQUIRE(*(++it1) == 64);
        REQUIRE(*(++it1) == 65);
        REQUIRE(*(++it1) == 96);
        REQUIRE(*(++it1) == 127);
        REQUIRE(++it1 == ni.cend());

        auto it2 = ni.crbegin();
        REQUIRE(*it2 == 127);
        REQUIRE(*(++it2) == 96);
        REQUIRE(*(++it2) == 65);
        REQUIRE(*(++it2) == 64);
        REQUIRE(*(++it2) == 2);
        REQUIRE(*(++it2) == 1);
        REQUIRE(++it2 == ni.crend());
    }

    SECTION("remove existing note from back") {
        REQUIRE(ni.remove(127) == true);
        REQUIRE(ni.size() == 6);

        REQUIRE(ni.front() == 0);
        REQUIRE(ni.back() == 96);

        auto it1 = ni.cbegin();
        REQUIRE(*it1 == 0);
        REQUIRE(*(++it1) == 1);
        REQUIRE(*(++it1) == 2);
        REQUIRE(*(++it1) == 64);
        REQUIRE(*(++it1) == 65);
        REQUIRE(*(++it1) == 96);
        REQUIRE(++it1 == ni.cend());

        auto it2 = ni.crbegin();
        REQUIRE(*it2 == 96);
        REQUIRE(*(++it2) == 65);
        REQUIRE(*(++it2) == 64);
        REQUIRE(*(++it2) == 2);
        REQUIRE(*(++it2) == 1);
        REQUIRE(*(++it2) == 0);
        REQUIRE(++it2 == ni.crend());
    }

    SECTION("remove all notes") {
        REQUIRE(ni.remove(127) == true);
        REQUIRE(ni.size() == 6);
        REQUIRE(ni.front() == 0);
        REQUIRE(ni.back() == 96);
        REQUIRE(ni.remove(127) == false);

        REQUIRE(ni.remove(0) == true);
        REQUIRE(ni.size() == 5);
        REQUIRE(ni.front() == 1);
        REQUIRE(ni.back() == 96);
        REQUIRE(ni.remove(0) == false);

        REQUIRE(ni.remove(96) == true);
        REQUIRE(ni.size() == 4);
        REQUIRE(ni.front() == 1);
        REQUIRE(ni.back() == 65);
        REQUIRE(ni.remove(96) == false);

        REQUIRE(ni.remove(1) == true);
        REQUIRE(ni.size() == 3);
        REQUIRE(ni.front() == 2);
        REQUIRE(ni.back() == 65);
        REQUIRE(ni.remove(1) == false);

        REQUIRE(ni.remove(65) == true);
        REQUIRE(ni.size() == 2);
        REQUIRE(ni.front() == 2);
        REQUIRE(ni.back() == 64);
        REQUIRE(ni.remove(65) == false);

        REQUIRE(ni.remove(2) == true);
        REQUIRE(ni.size() == 1);
        REQUIRE(ni.front() == 64);
        REQUIRE(ni.back() == 64);
        REQUIRE(ni.remove(2) == false);

        REQUIRE(ni.remove(64) == true);
        REQUIRE(ni.size() == 0);
        REQUIRE(ni.front() == NO_NOTE);
        REQUIRE(ni.back() == NO_NOTE);
        REQUIRE(ni.cbegin() == ni.cend());
        REQUIRE(ni.crbegin() == ni.crend());
        REQUIRE(ni.remove(64) == false);
    }
}

TEST_CASE("insert every possible note", TEST_GROUP) {
    NoteIndex ni;
    for (NoteNumber n = 0; isValidNote(n); n++) {
        ni.insert(n);
    }
    REQUIRE(ni.size() == 128);
    REQUIRE(ni.front() == 0);
    REQUIRE(ni.back() == 127);
}

TEST_CASE("clear", TEST_GROUP) {
    NoteIndex ni;
    for (NoteNumber n = 0; isValidNote(n); n++) {
        ni.insert(n);
    }
    ni.clear();

    REQUIRE(ni.size() == 0);
    REQUIRE(ni.front() == NO_NOTE);
    REQUIRE(ni.back() == NO_NOTE);
    REQUIRE(ni.cbegin() == ni.cend());
    REQUIRE(ni.crbegin() == ni.crend());
}
