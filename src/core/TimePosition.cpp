
#include "TimePosition.h"

bool TimePosition::operator<(const TimePosition other) const {
    return value < other.value;
}

bool TimePosition::operator<=(const TimePosition other) const {
    return value <= other.value;
}

bool TimePosition::operator>(const TimePosition other) const {
    return value > other.value;
}

bool TimePosition::operator>=(const TimePosition other) const {
    return value >= other.value;
}

bool TimePosition::operator==(const TimePosition other) const {
    return value == other.value;
}

int TimePosition::compare(TimePosition other) const {
    if (value < other.value) return -1;
    else if (value > other.value) return 1;
    else return 0;
}

TimePosition TimePosition::next() const {
    return TimePosition(value + 1);
}

TimePosition TimePositionTracker::update(int sampleNumber) {
    if (m_lastSample < 0) {
        m_lastSample = sampleNumber;
    } else if (sampleNumber > m_lastSample) {
        m_lastSample = sampleNumber;
        m_current = m_current.next();
    }
    return m_current;
}

void TimePositionTracker::newFrame() {
    m_lastSample = -1;
}
