
#include "Parameters.h"

int Parameters::maxVoices() const {
    return m_maxVoices;
}

void Parameters::setMaxVoices(int value) {
    if (value < 0) value = 0;
    if (value > 128) value = 128;
    m_maxVoices = value;
}

Stealing::Strategy Parameters::stealingStrategy() const {
    return m_stealingStrategy;
}

void Parameters::setStealingStrategy(Stealing::Strategy value) {
    m_stealingStrategy = value;
}

const SpecialKeys &Parameters::specialKeys() const {
    return m_specialKeys;
}
