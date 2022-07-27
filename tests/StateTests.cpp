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