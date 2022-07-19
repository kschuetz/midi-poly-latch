//
// Created by kschu on 6/30/2022.
//

#include "State.h"

void State::activateNote(const NoteNumber noteNumber, const Velocity velocity, const Timestamp ts) {
    if (notes[noteNumber].status == NoteStatus::NotPlaying) {
        activeNotes.add(noteNumber);
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
    activeNotes.remove(noteNumber);
}

void State::clear() {
    activeNotes.clear();
    for (auto &note: notes) {
        note.status = NoteStatus::NotPlaying;
    }
}

const NoteList &State::getActiveNotes() {
    return activeNotes;
}

int State::getPlayingCount() {
    return activeNotes.getSize();
}

NoteStatus State::getStatus(NoteNumber noteNumber) {
    return notes[noteNumber].status;
}


