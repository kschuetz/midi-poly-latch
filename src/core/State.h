#pragma once

#include "Consts.h"
#include "NoteState.h"
#include "NoteList.h"
#include "NoteIndex.h"

class State final {
public:
    void activateNote(NoteNumber noteNumber, Velocity velocity, Timestamp ts);

    void deactivateNote(NoteNumber noteNumber);

    void beginRelease(NoteNumber noteNumber, Velocity velocity, Timestamp ts);

    void clear();

    [[nodiscard]] int playingCount() const;

    [[nodiscard]] NoteStatus noteStatus(NoteNumber noteNumber) const;

    [[nodiscard]] const NoteState &noteState(NoteNumber noteNumber) const;

    [[nodiscard]] const NoteIndex &noteIndex() const {
        return m_noteIndex;
    }

private:
    NoteState m_notes[KEY_COUNT];
    NoteIndex m_noteIndex;
};
