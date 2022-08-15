
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

