
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

SpecialKeys Parameters::specialKeys() const {
    return m_specialKeys;
}

void Parameters::setSpecialKeys(SpecialKeys value) {
    m_specialKeys = value;
}

ReleaseTrigger Parameters::releaseTrigger() const {
    return m_releaseTrigger;
}

void Parameters::setReleaseTrigger(ReleaseTrigger value) {
    m_releaseTrigger = value;
}

ReleaseVelocitySource Parameters::releaseVelocitySource() const {
    return m_releaseVelocitySource;
}

void Parameters::setReleaseVelocitySource(ReleaseVelocitySource value) {
    m_releaseVelocitySource = value;
}
