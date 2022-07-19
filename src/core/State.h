#pragma once

#include "Consts.h"
#include "NoteState.h"
#include "NoteList.h"

class State {
public:
    void activateNote(NoteNumber noteNumber, Velocity velocity, Timestamp ts);

    void deactivateNote(NoteNumber noteNumber);

    void beginRelease(NoteNumber noteNumber, Velocity velocity, Timestamp ts);

    void clear();

    const NoteList &getActiveNotes();

    int getPlayingCount();

    NoteStatus getStatus(NoteNumber noteNumber);

private:
    NoteState notes[KEY_COUNT];
    NoteList activeNotes;
};
