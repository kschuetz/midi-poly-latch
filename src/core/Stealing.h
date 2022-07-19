#pragma once

#include <vector>
#include <memory>

#include "Consts.h"
#include "NoteState.h"
#include "NoteList.h"

namespace Stealing {
    class Strategy {
        virtual void selectNotesToSteal(NoteState(&noteStates)[KEY_COUNT], int numNotesToSteal, NoteList &result) = 0;

    public:
        virtual ~Strategy();
    };

    class Oldest final : public Strategy {
        void selectNotesToSteal(NoteState(&noteStates)[KEY_COUNT], int numNotesToSteal, NoteList &result) override;
    };

    class Newest final : public Strategy {
        void selectNotesToSteal(NoteState(&noteStates)[KEY_COUNT], int numNotesToSteal, NoteList &result) override;
    };

    std::vector<std::unique_ptr<Strategy>> allStrategies();
}