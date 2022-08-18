#pragma once

#include <compare>

using NoteNumber = signed char;

constexpr NoteNumber NO_NOTE = -1;

constexpr NoteNumber MIN_NOTE = 0;
constexpr NoteNumber MAX_NOTE = 127;

inline bool isValidNote(NoteNumber noteNumber) {
    return noteNumber >= 0;
}

class Velocity final {
public:
    explicit Velocity(unsigned char value) : m_value(value & 127) {}

    Velocity &operator=(const Velocity other) {
        m_value = other.m_value;
        return *this;
    }

    bool operator==(Velocity other) const;

    [[nodiscard]] unsigned char value() const {
        return m_value;
    }

private:
    unsigned char m_value;
};

#define MaxVelocity Velocity(127)

enum class NoteStatus {
    NotPlaying,
    Playing,
    ReadyToRelease
};
