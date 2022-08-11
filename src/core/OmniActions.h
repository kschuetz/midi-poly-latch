#pragma once

#include "NoteList.h"

/**
 * Actions triggered in the UI that affect all channels (e.g. keypress on on-screen keyboard)
 */
class OmniActions {
public:
    void reset();

    void triggerNoteOn(NoteNumber note);

    void triggerNoteOff(NoteNumber note);

    void triggerAllNotesOff();

    void triggerOneNoteOff();

    [[nodiscard]] bool isBufferWriteNeeded() const;

private:
    NoteList m_noteOn;
    NoteList m_noteOff;
    bool m_allNotesOff;
    bool m_oneNoteOff;
};
