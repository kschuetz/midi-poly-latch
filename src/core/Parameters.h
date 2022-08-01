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

    [[nodiscard]] SpecialKeys specialKeys() const;

    void setSpecialKeys(SpecialKeys value);

    [[nodiscard]] ReleaseTrigger releaseTrigger() const;

    void setReleaseTrigger(ReleaseTrigger value);

    [[nodiscard]] ReleaseVelocitySource releaseVelocitySource() const;

    void setReleaseVelocitySource(ReleaseVelocitySource value);

private:
    int m_maxVoices = defaultMaxVoices;
    Stealing::Strategy m_stealingStrategy = defaultStealingStrategy;
    SpecialKeys m_specialKeys{};
    ReleaseTrigger m_releaseTrigger{};
    ReleaseVelocitySource m_releaseVelocitySource{};
};
