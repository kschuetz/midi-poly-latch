#pragma once

#include <vector>
#include <memory>

#include "Consts.h"
#include "NoteState.h"
#include "ChannelState.h"
#include "NoteList.h"
#include "Rng.h"

namespace Stealing {

    enum class PrimaryStrategy {
        Oldest,
        Newest,
        Lowest,
        Highest,
        MostDistant,
        LeastDistant,
        MostCentral,
        Random,
        RandomInner,
        RandomOuter,
    };

    enum class SecondaryStrategy {
        StealRandom,
        StealLower,
        StealHigher
    };

    struct Strategy {
        PrimaryStrategy primary;
        SecondaryStrategy secondary;
    };

}

NoteNumber
selectNoteToSteal(const Stealing::Strategy &strategy, const ChannelState &state, Rng &rng, NoteNumber userNotePlayed);
