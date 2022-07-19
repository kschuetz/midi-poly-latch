#pragma once

using NoteNumber = signed char;

constexpr NoteNumber NO_NOTE = -1;

inline bool isValidNote(NoteNumber noteNumber) {
    return noteNumber >= 0;
}

class Timestamp final {
public:
    explicit Timestamp(const unsigned int value) : value(value) {}

    Timestamp &operator=(const Timestamp other) {
        value = other.value;
        return *this;
    }

    unsigned int value;
};

class Velocity final {
public:
    explicit Velocity(char value) : value(value & 0x7F) {}

    Velocity &operator=(const Velocity other) {
        value = other.value;
        return *this;
    }

    char value;
};


enum class NoteStatus {
    NotPlaying,
    Playing,
    ReadyToRelease
};
