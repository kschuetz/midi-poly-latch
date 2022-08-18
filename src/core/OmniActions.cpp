
#include "OmniActions.h"

void OmniActions::reset() {
    m_toggledNotes.clear();
    m_allNotesOff = false;
    m_oneNoteOff = false;
}

void OmniActions::triggerToggleNote(NoteNumber note) {
    m_toggledNotes.add(note);
}

void OmniActions::triggerAllNotesOff() {
    m_allNotesOff = true;
}

void OmniActions::triggerOneNoteOff() {
    m_oneNoteOff = true;
}

bool OmniActions::isBufferWriteNeeded() const {
    return m_allNotesOff || m_oneNoteOff || !m_toggledNotes.isEmpty();
}

const NoteList &OmniActions::toggledNotes() const {
    return m_toggledNotes;
}

bool OmniActions::wasOneNoteOffTriggered() const {
    return m_oneNoteOff;
}

bool OmniActions::wasAllNotesOffTriggered() const {
    return m_allNotesOff;
}
