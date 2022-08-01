#pragma once

#include <compare>

using NoteNumber = signed char;

constexpr NoteNumber NO_NOTE = -1;

constexpr NoteNumber MIN_NOTE = 0;
constexpr NoteNumber MAX_NOTE = 127;

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

    [[nodiscard]] int compare(Timestamp other) const;

    bool operator==(Timestamp other) const;

    bool operator<(Timestamp other) const;

    bool operator<=(Timestamp other) const;

    bool operator>(Timestamp other) const;

    bool operator>=(Timestamp other) const;

    unsigned int value;
};

class Velocity final {
public:
    explicit Velocity(signed char value) : value(value < 0 ? 0 : value) {}

    Velocity &operator=(const Velocity other) {
        value = other.value;
        return *this;
    }

    bool operator==(Velocity other) const;

    signed char value;
};

#define MaxVelocity Velocity(127)

enum class NoteStatus {
    NotPlaying,
    Playing,
    ReadyToRelease
};
