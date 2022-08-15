#pragma once

class TimePosition final {
public:
    explicit TimePosition(const unsigned int value) : value(value) {}

    TimePosition &operator=(const TimePosition other) {
        value = other.value;
        return *this;
    }

    [[nodiscard]] int compare(TimePosition other) const;

    bool operator==(TimePosition other) const;

    bool operator<(TimePosition other) const;

    bool operator<=(TimePosition other) const;

    bool operator>(TimePosition other) const;

    bool operator>=(TimePosition other) const;

    unsigned int value;
};