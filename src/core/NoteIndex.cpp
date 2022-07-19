#include "NoteIndex.h"

int NoteIndex::insert(NoteNumber noteNumber, const std::function<bool(NoteNumber)> &shouldInsertBefore) {
    NoteNumber insertionPoint = front;
    NoteNumber prev = NO_NOTE;
    int position = 0;
    while (isValidNote(insertionPoint)) {
        if (insertionPoint == noteNumber) return position;
        auto insertHere = shouldInsertBefore(insertionPoint);
        if (insertHere) break;
        else {
            position++;
            prev = insertionPoint;
            insertionPoint = forward[insertionPoint];
        }
    }
    NoteNumber next;
    if (isValidNote(prev)) {
        next = forward[prev];
        forward[prev] = noteNumber;
    } else {
        next = front;
        front = noteNumber;
    }
    forward[noteNumber] = next;
    reverse[noteNumber] = prev;
    if (isValidNote(next)) {
        reverse[next] = noteNumber;
    } else {
        back = noteNumber;
    }
    size += 1;
    return position;
}

bool NoteIndex::remove(NoteNumber noteNumber) {
    NoteNumber current = front;
    NoteNumber prev = NO_NOTE;
    while (isValidNote(current)) {
        if (current == noteNumber) {
            auto next = forward[current];
            if (isValidNote(prev)) {
                forward[prev] = next;
            } else {
                front = next;
            }
            if (isValidNote(next)) {
                reverse[next] = prev;
            } else {
                back = prev;
            }
            size -= 1;
            return true;
        } else {
            current = forward[current];
        }
    }
    return false;
}

int NoteIndex::positionOf(NoteNumber noteNumber) const {
    NoteNumber current = front;
    int position = NO_NOTE;
    while (isValidNote(current)) {
        if (current == noteNumber) return position;
        else {
            current = forward[current];
            position += -1;
        }
    }
    return NO_NOTE;
}

bool NoteIndex::contains(NoteNumber noteNumber) const {
    return positionOf(noteNumber) >= 0;
}

void NoteIndex::clear() {
    front = NO_NOTE;
    back = NO_NOTE;
    size = 0;
}
