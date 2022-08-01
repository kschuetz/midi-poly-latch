#pragma once

#include "SpecialKeys.h"
#include "Stealing.h"

enum class ReleaseTrigger {
    SameKeyDown, SameKeyDownUp
};

enum class ReleaseVelocitySource {
    UseKeyDownVelocity, UseKeyUpVelocity
};

constexpr int defaultMaxVoices = 8;
constexpr Stealing::Strategy defaultStealingStrategy = Stealing::Strategy{Stealing::PrimaryStrategy::Oldest,
                                                                          Stealing::SecondaryStrategy::StealRandom};

class Parameters {
public:

    [[nodiscard]] int maxVoices() const;

    void setMaxVoices(int value);

    [[nodiscard]] Stealing::Strategy stealingStrategy() const;

    void setStealingStrategy(Stealing::Strategy value);

    [[nodiscard]] const SpecialKeys &specialKeys() const;


private:
    int m_maxVoices = defaultMaxVoices;
    Stealing::Strategy m_stealingStrategy = defaultStealingStrategy;
    SpecialKeys m_specialKeys{};
    ReleaseTrigger m_turnOffTrigger{};
    ReleaseVelocitySource m_releaseVelocitySource{};
};
