
#include "OmniActions.h"

void OmniActions::reset() {
    m_notesOff.clear();
    m_notesOn.clear();
    m_allNotesOff = false;
    m_oneNoteOff = false;
}

void OmniActions::triggerNoteOn(NoteNumber note) {
    m_notesOn.add(note);
}

void OmniActions::triggerNoteOff(NoteNumber note) {
    m_notesOff.add(note);
}

void OmniActions::triggerAllNotesOff() {
    m_allNotesOff = true;
}

void OmniActions::triggerOneNoteOff() {
    m_oneNoteOff = true;
}

bool OmniActions::isBufferWriteNeeded() const {
    return m_allNotesOff || m_oneNoteOff;
}

const NoteList &OmniActions::notesToTurnOn() const {
    return m_notesOn;
}

const NoteList &OmniActions::notesToTurnOff() const {
    return m_notesOff;
}

bool OmniActions::wasOneNoteOffTriggered() const {
    return m_oneNoteOff;
}

bool OmniActions::wasAllNotesOffTriggered() const {
    return m_allNotesOff;
}
