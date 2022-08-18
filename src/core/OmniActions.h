#pragma once

#include "NoteList.h"

/**
 * Actions triggered in the UI that affect all channels (e.g. keypress on on-screen keyboard)
 */
class OmniActions final {
public:
    void reset();

    /**
     * User clicked the on-screen keyboard for the given note
     */
    void triggerToggleNote(NoteNumber note);

    /**
     * User clicked the "all notes off" button in the UI
     */
    void triggerAllNotesOff();

    /**
     * User clicked the "one note off" button in the UI
     */
    void triggerOneNoteOff();

    [[nodiscard]] bool isBufferWriteNeeded() const;

    [[nodiscard]] const NoteList &toggledNotes() const;

    [[nodiscard]] bool wasAllNotesOffTriggered() const;

    [[nodiscard]] bool wasOneNoteOffTriggered() const;

private:
    NoteList m_toggledNotes{};
    bool m_allNotesOff = false;
    bool m_oneNoteOff = false;
};
