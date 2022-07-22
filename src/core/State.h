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

    int getPlayingCount();

    NoteStatus getStatus(NoteNumber noteNumber);

    [[nodiscard]] const NoteState &getNoteState(NoteNumber noteNumber) const;

    [[nodiscard]] const NoteIndexRead &indexByPitch() const {
        return m_indexByPitch;
    }

    [[nodiscard]] const NoteIndexRead &indexByAge() const {
        return m_indexByAge;
    }

private:
    NoteState notes[KEY_COUNT];
    NoteIndex m_indexByPitch;
    NoteIndex m_indexByAge;
};
