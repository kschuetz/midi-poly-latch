
#include "OmniActions.h"

void OmniActions::reset() {

}

void OmniActions::triggerNoteOn(NoteNumber note) {

}

void OmniActions::triggerNoteOff(NoteNumber note) {

}

void OmniActions::triggerAllNotesOff() {

}

void OmniActions::triggerOneNoteOff() {

}

bool OmniActions::isBufferWriteNeeded() const {
    return m_allNotesOff || m_oneNoteOff;
}
