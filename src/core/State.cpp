#include "State.h"

void State::activateNote(const NoteNumber noteNumber, const Velocity velocity, const Timestamp ts) {
    if (notes[noteNumber].status == NoteStatus::NotPlaying) {
        m_indexByPitch.insert(noteNumber, [noteNumber](NoteNumber other) -> bool { return noteNumber < other; });
        m_indexByAge.insert(noteNumber,
                            [ts, this, noteNumber](NoteNumber other) -> bool {
                                auto compared = ts.compare(notes[other].startedPlaying);
                                if (compared == 0) return noteNumber < other;
                                else return compared < 0;
                            });
    }
    auto &noteState = notes[noteNumber];
    noteState.status = NoteStatus::Playing;
    noteState.initialDownVelocity = velocity;
    noteState.startedPlaying = ts;
}

void State::beginRelease(NoteNumber noteNumber, Velocity velocity, Timestamp ts) {
    if (notes[noteNumber].status == NoteStatus::Playing) {
        auto &noteState = notes[noteNumber];
        noteState.status = NoteStatus::Playing;
        noteState.initialDownVelocity = velocity;
        noteState.beganRelease = ts;
    }
}

void State::deactivateNote(NoteNumber noteNumber) {
    notes[noteNumber].status = NoteStatus::NotPlaying;
    m_indexByPitch.remove(noteNumber);
    m_indexByAge.remove(noteNumber);
}

void State::clear() {
    m_indexByPitch.clear();
    m_indexByAge.clear();
    for (auto &note: notes) {
        note.status = NoteStatus::NotPlaying;
    }
}

int State::getPlayingCount() {
    return m_indexByPitch.getSize();
}

NoteStatus State::getStatus(NoteNumber noteNumber) {
    return notes[noteNumber].status;
}

const NoteState &State::getNoteState(NoteNumber noteNumber) const {
    return notes[noteNumber];
}
