#include <catch2/catch_all.hpp>
#include "core/State.h"
#include "support/NoteIndexMatchers.h"

#define STATE_TAG "[State]"

TEST_CASE("default State", STATE_TAG) {
    State state;

    CHECK(state.playingCount() == 0);
    for (NoteNumber n = 0; isValidNote(n); n++) {
        CHECK(state.noteStatus(n) == NoteStatus::NotPlaying);
    }
    CHECK(state.noteIndex().cbegin() == state.noteIndex().cend());
    CHECK(state.noteIndex().crbegin() == state.noteIndex().crend());
}

TEST_CASE("activateNote", STATE_TAG) {
    State state;

    SECTION("one note") {
        state.activateNote(64, MaxVelocity, Timestamp(1));

        CHECK(state.playingCount() == 1);
        CHECK(state.noteStatus(64) == NoteStatus::Playing);
        CHECK(state.noteState(64).initialDownVelocity == MaxVelocity);
        CHECK(state.noteState(64).startedPlaying == Timestamp(1));

        CHECK_THAT(state.noteIndex(), IteratesForward({64}));
        CHECK_THAT(state.noteIndex(), IteratesReverse({64}));
    }

    SECTION("activating same note twice replaces velocity and timestamp") {
        state.activateNote(64, MaxVelocity, Timestamp(1));
        state.activateNote(64, Velocity(100), Timestamp(2));

        CHECK(state.playingCount() == 1);
        CHECK(state.noteStatus(64) == NoteStatus::Playing);
        CHECK(state.noteState(64).initialDownVelocity == Velocity(100));
        CHECK(state.noteState(64).startedPlaying == Timestamp(2));

        CHECK_THAT(state.noteIndex(), IteratesForward({64}));
        CHECK_THAT(state.noteIndex(), IteratesReverse({64}));
    }

    SECTION("multiple notes") {
        state.activateNote(127, Velocity(100), Timestamp(10));
        state.activateNote(0, Velocity(101), Timestamp(9));
        state.activateNote(64, Velocity(102), Timestamp(8));
        state.activateNote(65, Velocity(103), Timestamp(7));

        CHECK(state.playingCount() == 4);

        CHECK(state.noteStatus(127) == NoteStatus::Playing);
        CHECK(state.noteState(127).initialDownVelocity == Velocity(100));
        CHECK(state.noteState(127).startedPlaying == Timestamp(10));

        CHECK(state.noteStatus(0) == NoteStatus::Playing);
        CHECK(state.noteState(0).initialDownVelocity == Velocity(101));
        CHECK(state.noteState(0).startedPlaying == Timestamp(9));

        CHECK(state.noteStatus(64) == NoteStatus::Playing);
        CHECK(state.noteState(64).initialDownVelocity == Velocity(102));
        CHECK(state.noteState(64).startedPlaying == Timestamp(8));

        CHECK(state.noteStatus(65) == NoteStatus::Playing);
        CHECK(state.noteState(65).initialDownVelocity == Velocity(103));
        CHECK(state.noteState(65).startedPlaying == Timestamp(7));

        CHECK_THAT(state.noteIndex(), IteratesForward({0, 64, 65, 127}));
        CHECK_THAT(state.noteIndex(), IteratesReverse({127, 65, 64, 0}));

        for (NoteNumber n = 0; isValidNote(n); n++) {
            if (!state.noteIndex().contains(n)) {
                CHECK(state.noteStatus(n) == NoteStatus::NotPlaying);
            }
        }
    }

    SECTION("all notes") {
        for (NoteNumber n = 0; isValidNote(n); n++) {
            state.activateNote(n, MaxVelocity, Timestamp(1));
        }

        CHECK(state.playingCount() == 128);

        for (NoteNumber n = 0; isValidNote(n); n++) {
            CHECK(state.noteStatus(n) == NoteStatus::Playing);
            CHECK(state.noteState(n).initialDownVelocity == MaxVelocity);
            CHECK(state.noteState(0).startedPlaying == Timestamp(1));
        }
    }
}
