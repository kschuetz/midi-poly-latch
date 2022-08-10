#include "ChannelState.h"

void ChannelState::activateNote(const NoteNumber noteNumber, const Velocity velocity, const Timestamp ts) {
    if (m_notes[noteNumber].status == NoteStatus::NotPlaying) {
        m_noteIndex.insert(noteNumber);
    }
    auto &noteState = m_notes[noteNumber];
    noteState.status = NoteStatus::Playing;
    noteState.initialDownVelocity = velocity;
    noteState.startedPlaying = ts;
}

void ChannelState::beginRelease(NoteNumber noteNumber, Velocity velocity, Timestamp ts) {
    if (m_notes[noteNumber].status == NoteStatus::Playing) {
        auto &noteState = m_notes[noteNumber];
        noteState.status = NoteStatus::Playing;
        noteState.initialDownVelocity = velocity;
        noteState.beganRelease = ts;
    }
}

void ChannelState::deactivateNote(NoteNumber noteNumber) {
    m_notes[noteNumber].status = NoteStatus::NotPlaying;
    m_noteIndex.remove(noteNumber);
}

void ChannelState::clear() {
    m_noteIndex.clear();
    for (auto &note: m_notes) {
        note.status = NoteStatus::NotPlaying;
    }
}

int ChannelState::playingCount() const {
    return m_noteIndex.size();
}

NoteStatus ChannelState::noteStatus(NoteNumber noteNumber) const {
    return m_notes[noteNumber].status;
}

const NoteState &ChannelState::noteState(NoteNumber noteNumber) const {
    return m_notes[noteNumber];
}