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

TEST_CASE("deactivateNote", STATE_TAG) {
    State state;

    state.activateNote(0, MaxVelocity, Timestamp(1));
    state.activateNote(1, MaxVelocity, Timestamp(1));
    state.activateNote(2, MaxVelocity, Timestamp(1));
    state.activateNote(64, MaxVelocity, Timestamp(1));
    state.activateNote(65, MaxVelocity, Timestamp(1));
    state.activateNote(96, MaxVelocity, Timestamp(1));
    state.activateNote(127, MaxVelocity, Timestamp(1));

    SECTION("deactivate note that isn't active") {
        state.deactivateNote(10);
        CHECK(state.playingCount() == 7);
        CHECK(state.noteStatus(0) == NoteStatus::Playing);
        CHECK(state.noteStatus(1) == NoteStatus::Playing);
        CHECK(state.noteStatus(2) == NoteStatus::Playing);
        CHECK(state.noteStatus(64) == NoteStatus::Playing);
        CHECK(state.noteStatus(65) == NoteStatus::Playing);
        CHECK(state.noteStatus(96) == NoteStatus::Playing);
        CHECK(state.noteStatus(127) == NoteStatus::Playing);
        CHECK_THAT(state.noteIndex(), IteratesForward({0, 1, 2, 64, 65, 96, 127}));
    }

    SECTION("deactivate note in front") {
        state.deactivateNote(0);
        CHECK(state.playingCount() == 6);
        CHECK(state.noteStatus(0) == NoteStatus::NotPlaying);
        CHECK(state.noteStatus(1) == NoteStatus::Playing);
        CHECK(state.noteStatus(2) == NoteStatus::Playing);
        CHECK(state.noteStatus(64) == NoteStatus::Playing);
        CHECK(state.noteStatus(65) == NoteStatus::Playing);
        CHECK(state.noteStatus(96) == NoteStatus::Playing);
        CHECK(state.noteStatus(127) == NoteStatus::Playing);
        CHECK_THAT(state.noteIndex(), IteratesForward({1, 2, 64, 65, 96, 127}));
    }

    SECTION("deactivate note in middle") {
        state.deactivateNote(64);
        CHECK(state.playingCount() == 6);
        CHECK(state.noteStatus(0) == NoteStatus::Playing);
        CHECK(state.noteStatus(1) == NoteStatus::Playing);
        CHECK(state.noteStatus(2) == NoteStatus::Playing);
        CHECK(state.noteStatus(64) == NoteStatus::NotPlaying);
        CHECK(state.noteStatus(65) == NoteStatus::Playing);
        CHECK(state.noteStatus(96) == NoteStatus::Playing);
        CHECK(state.noteStatus(127) == NoteStatus::Playing);
        CHECK_THAT(state.noteIndex(), IteratesForward({0, 1, 2, 65, 96, 127}));
    }

    SECTION("deactivate note in back") {
        state.deactivateNote(127);
        CHECK(state.playingCount() == 6);
        CHECK(state.noteStatus(0) == NoteStatus::Playing);
        CHECK(state.noteStatus(1) == NoteStatus::Playing);
        CHECK(state.noteStatus(2) == NoteStatus::Playing);
        CHECK(state.noteStatus(64) == NoteStatus::Playing);
        CHECK(state.noteStatus(65) == NoteStatus::Playing);
        CHECK(state.noteStatus(96) == NoteStatus::Playing);
        CHECK(state.noteStatus(127) == NoteStatus::NotPlaying);
        CHECK_THAT(state.noteIndex(), IteratesForward({0, 1, 2, 64, 65, 96}));
    }

    SECTION("deactivate multiple notes") {
        state.deactivateNote(1);
        state.deactivateNote(64);
        state.deactivateNote(96);
        CHECK(state.playingCount() == 4);
        CHECK(state.noteStatus(0) == NoteStatus::Playing);
        CHECK(state.noteStatus(1) == NoteStatus::NotPlaying);
        CHECK(state.noteStatus(2) == NoteStatus::Playing);
        CHECK(state.noteStatus(64) == NoteStatus::NotPlaying);
        CHECK(state.noteStatus(65) == NoteStatus::Playing);
        CHECK(state.noteStatus(96) == NoteStatus::NotPlaying);
        CHECK(state.noteStatus(127) == NoteStatus::Playing);
        CHECK_THAT(state.noteIndex(), IteratesForward({0, 2, 65, 127}));
    }

}

TEST_CASE("clear State", STATE_TAG) {
    State state;

    state.activateNote(0, MaxVelocity, Timestamp(1));
    state.activateNote(1, MaxVelocity, Timestamp(1));
    state.activateNote(2, MaxVelocity, Timestamp(1));
    state.activateNote(64, MaxVelocity, Timestamp(1));
    state.activateNote(65, MaxVelocity, Timestamp(1));
    state.activateNote(96, MaxVelocity, Timestamp(1));
    state.activateNote(127, MaxVelocity, Timestamp(1));

    state.clear();

    CHECK(state.playingCount() == 0);
    for (NoteNumber n = 0; isValidNote(n); n++) {
        CHECK(state.noteStatus(n) == NoteStatus::NotPlaying);
    }
    CHECK_THAT(state.noteIndex(), IteratesForward({}));
}
