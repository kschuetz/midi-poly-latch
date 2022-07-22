#include "CommonTypes.h"

bool Timestamp::operator<(const Timestamp other) const {
    return value < other.value;
}

bool Timestamp::operator<=(const Timestamp other) const {
    return value <= other.value;
}

bool Timestamp::operator>(const Timestamp other) const {
    return value > other.value;
}

bool Timestamp::operator>=(const Timestamp other) const {
    return value >= other.value;
}

bool Timestamp::operator==(const Timestamp other) const {
    return value == other.value;
}

int Timestamp::compare(Timestamp other) const {
    if (value < other.value) return -1;
    else if (value > other.value) return 1;
    else return 0;
}

