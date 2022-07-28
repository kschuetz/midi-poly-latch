#include <catch2/catch_all.hpp>
#include "core/NoteIndex.h"
#include "support/NoteIndexMatchers.h"

#define NOTE_INDEX_TAG "[NoteIndex]"

TEST_CASE("inserts notes in the proper position", NOTE_INDEX_TAG) {
    NoteIndex ni;
    CHECK(ni.insert(0) == 0);
    CHECK(ni.positionOf(0) == 0);
    CHECK(ni.contains(0));
    CHECK(ni.size() == 1);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 0);

    CHECK(ni.insert(1) == 1);
    CHECK(ni.positionOf(1) == 1);
    CHECK(ni.contains(1));
    CHECK(ni.size() == 2);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 1);

    CHECK(ni.insert(0) == 0);
    CHECK(ni.positionOf(0) == 0);
    CHECK(ni.contains(0));
    CHECK(ni.size() == 2);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 1);

    CHECK(ni.insert(127) == 2);
    CHECK(ni.positionOf(127) == 2);
    CHECK(ni.contains(127));
    CHECK(ni.size() == 3);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 127);

    CHECK(ni.insert(64) == 2);
    CHECK(ni.positionOf(64) == 2);
    CHECK(ni.contains(64));
    CHECK(ni.size() == 4);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 127);

    CHECK(ni.insert(65) == 3);
    CHECK(ni.positionOf(65) == 3);
    CHECK(ni.contains(65));
    CHECK(ni.size() == 5);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 127);

    CHECK(ni.insert(96) == 4);
    CHECK(ni.positionOf(96) == 4);
    CHECK(ni.contains(96));
    CHECK(ni.size() == 6);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 127);

    CHECK(ni.insert(2) == 2);
    CHECK(ni.positionOf(2) == 2);
    CHECK(ni.contains(2));
    CHECK(ni.size() == 7);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 127);
}

TEST_CASE("iterates correctly", NOTE_INDEX_TAG) {
    NoteIndex ni;
    ni.insert(0);
    ni.insert(1);
    ni.insert(2);
    ni.insert(64);
    ni.insert(65);
    ni.insert(96);
    ni.insert(127);

    SECTION("forward") {
        CHECK_THAT(ni, IteratesForward({0, 1, 2, 64, 65, 96, 127}));
    }

    SECTION("reverse") {
        CHECK_THAT(ni, IteratesReverse({127, 96, 65, 64, 2, 1, 0}));
    }
}

TEST_CASE("empty", NOTE_INDEX_TAG) {
    NoteIndex ni;

    CHECK(ni.size() == 0);
    CHECK(ni.front() == NO_NOTE);
    CHECK(ni.back() == NO_NOTE);
    CHECK(ni.cbegin() == ni.cend());
    CHECK(ni.crbegin() == ni.crend());

    SECTION("contains no notes") {
        int containsCount = 0;
        for (NoteNumber n = 0; isValidNote(n); n++) {
            if (ni.contains(n)) containsCount++;
        }
        CHECK(containsCount == 0);
    }
}

TEST_CASE("insert does not accept illegal notes", NOTE_INDEX_TAG) {
    NoteIndex ni;
    CHECK(ni.insert(NO_NOTE) == -1);
    CHECK(ni.insert(NO_NOTE - 1) == -1);
}

TEST_CASE("removes notes", NOTE_INDEX_TAG) {
    NoteIndex ni;

    ni.insert(0);
    ni.insert(1);
    ni.insert(2);
    ni.insert(64);
    ni.insert(65);
    ni.insert(96);
    ni.insert(127);

    SECTION("remove non-existing note") {
        CHECK(ni.remove(126) == false);
        CHECK(ni.size() == 7);

        CHECK(ni.front() == 0);
        CHECK(ni.back() == 127);

        CHECK_THAT(ni, IteratesForward({0, 1, 2, 64, 65, 96, 127}));
        CHECK_THAT(ni, IteratesReverse({127, 96, 65, 64, 2, 1, 0}));
    }

    SECTION("remove existing note from middle") {
        CHECK(ni.remove(64) == true);
        CHECK(ni.size() == 6);

        CHECK(ni.front() == 0);
        CHECK(ni.back() == 127);

        CHECK_THAT(ni, IteratesForward({0, 1, 2, 65, 96, 127}));
        CHECK_THAT(ni, IteratesReverse({127, 96, 65, 2, 1, 0}));
    }

    SECTION("remove existing note from front") {
        CHECK(ni.remove(0) == true);
        CHECK(ni.size() == 6);

        CHECK(ni.front() == 1);
        CHECK(ni.back() == 127);

        CHECK_THAT(ni, IteratesForward({1, 2, 64, 65, 96, 127}));
        CHECK_THAT(ni, IteratesReverse({127, 96, 65, 64, 2, 1}));
    }

    SECTION("remove existing note from back") {
        CHECK(ni.remove(127) == true);
        CHECK(ni.size() == 6);

        CHECK(ni.front() == 0);
        CHECK(ni.back() == 96);

        CHECK_THAT(ni, IteratesForward({0, 1, 2, 64, 65, 96}));
        CHECK_THAT(ni, IteratesReverse({96, 65, 64, 2, 1, 0}));
    }

    SECTION("remove all notes") {
        CHECK(ni.remove(127) == true);
        CHECK(ni.size() == 6);
        CHECK(ni.front() == 0);
        CHECK(ni.back() == 96);
        CHECK(ni.remove(127) == false);

        CHECK(ni.remove(0) == true);
        CHECK(ni.size() == 5);
        CHECK(ni.front() == 1);
        CHECK(ni.back() == 96);
        CHECK(ni.remove(0) == false);

        CHECK(ni.remove(96) == true);
        CHECK(ni.size() == 4);
        CHECK(ni.front() == 1);
        CHECK(ni.back() == 65);
        CHECK(ni.remove(96) == false);

        CHECK(ni.remove(1) == true);
        CHECK(ni.size() == 3);
        CHECK(ni.front() == 2);
        CHECK(ni.back() == 65);
        CHECK(ni.remove(1) == false);

        CHECK(ni.remove(65) == true);
        CHECK(ni.size() == 2);
        CHECK(ni.front() == 2);
        CHECK(ni.back() == 64);
        CHECK(ni.remove(65) == false);

        CHECK(ni.remove(2) == true);
        CHECK(ni.size() == 1);
        CHECK(ni.front() == 64);
        CHECK(ni.back() == 64);
        CHECK(ni.remove(2) == false);

        CHECK(ni.remove(64) == true);
        CHECK(ni.size() == 0);
        CHECK(ni.front() == NO_NOTE);
        CHECK(ni.back() == NO_NOTE);
        CHECK(ni.cbegin() == ni.cend());
        CHECK(ni.crbegin() == ni.crend());
        CHECK(ni.remove(64) == false);
    }
}

TEST_CASE("insert every possible note", NOTE_INDEX_TAG) {
    NoteIndex ni;
    for (NoteNumber n = 0; isValidNote(n); n++) {
        ni.insert(n);
    }
    CHECK(ni.size() == 128);
    CHECK(ni.front() == 0);
    CHECK(ni.back() == 127);
}

TEST_CASE("clear", NOTE_INDEX_TAG) {
    NoteIndex ni;
    for (NoteNumber n = 0; isValidNote(n); n++) {
        ni.insert(n);
    }
    ni.clear();

    CHECK(ni.size() == 0);
    CHECK(ni.front() == NO_NOTE);
    CHECK(ni.back() == NO_NOTE);
    CHECK(ni.cbegin() == ni.cend());
    CHECK(ni.crbegin() == ni.crend());
}
