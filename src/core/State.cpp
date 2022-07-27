#include "State.h"

void State::activateNote(const NoteNumber noteNumber, const Velocity velocity, const Timestamp ts) {
    if (m_notes[noteNumber].status == NoteStatus::NotPlaying) {
        m_noteIndex.insert(noteNumber);
    }
    auto &noteState = m_notes[noteNumber];
    noteState.status = NoteStatus::Playing;
    noteState.initialDownVelocity = velocity;
    noteState.startedPlaying = ts;
}

void State::beginRelease(NoteNumber noteNumber, Velocity velocity, Timestamp ts) {
    if (m_notes[noteNumber].status == NoteStatus::Playing) {
        auto &noteState = m_notes[noteNumber];
        noteState.status = NoteStatus::Playing;
        noteState.initialDownVelocity = velocity;
        noteState.beganRelease = ts;
    }
}

void State::deactivateNote(NoteNumber noteNumber) {
    m_notes[noteNumber].status = NoteStatus::NotPlaying;
    m_noteIndex.remove(noteNumber);
}

void State::clear() {
    m_noteIndex.clear();
    for (auto &note: m_notes) {
        note.status = NoteStatus::NotPlaying;
    }
}

int State::playingCount() const {
    return m_noteIndex.size();
}

NoteStatus State::noteStatus(NoteNumber noteNumber) const {
    return m_notes[noteNumber].status;
}

const NoteState &State::noteState(NoteNumber noteNumber) const {
    return m_notes[noteNumber];
}
