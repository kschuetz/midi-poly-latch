#pragma once

#include "CommonTypes.h"

constexpr NoteNumber defaultLatchOverrideKey = NO_NOTE;
constexpr NoteNumber defaultAllNotesOffKey = NO_NOTE;
constexpr NoteNumber defaultOneNoteOffKey = NO_NOTE;

struct SpecialKeys {
    NoteNumber latchOverride = defaultAllNotesOffKey;
    NoteNumber allNotesOff = defaultAllNotesOffKey;
    NoteNumber oneNoteOff = defaultOneNoteOffKey;
};
