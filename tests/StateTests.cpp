#include <catch2/catch_all.hpp>
#include "core/State.h"

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

    state.activateNote(64, MaxVelocity, Timestamp(1));

    CHECK(state.playingCount() == 1);
    CHECK(state.noteStatus(64) == NoteStatus::Playing);
    CHECK(state.noteState(64).initialDownVelocity == MaxVelocity);
    CHECK(state.noteState(64).startedPlaying == Timestamp(1));

}