#pragma once

class TimePosition final {
public:
    explicit TimePosition(const unsigned int value) : value(value) {}

    TimePosition &operator=(const TimePosition other) {
        value = other.value;
        return *this;
    }

    [[nodiscard]] int compare(TimePosition other) const;

    [[nodiscard]] TimePosition next() const;

    bool operator==(TimePosition other) const;

    bool operator<(TimePosition other) const;

    bool operator<=(TimePosition other) const;

    bool operator>(TimePosition other) const;

    bool operator>=(TimePosition other) const;

    unsigned int value;
};

class TimePositionTracker final {
public:
    TimePositionTracker() : m_current(TimePosition(0)), m_lastSample(-1) {};

    TimePosition update(int sampleNumber);

    void newFrame();

private:
    TimePosition m_current;
    int m_lastSample;
};
