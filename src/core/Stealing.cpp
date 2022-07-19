#include "Stealing.h"

namespace Stealing {

    void Oldest::selectNotesToSteal(NoteState (&noteStates)[KEY_COUNT], int numNotesToSteal, NoteList &result) {
        if (numNotesToSteal <= 0) return;
        if (numNotesToSteal == 1) {

        }
    }

    void Newest::selectNotesToSteal(NoteState (&noteStates)[KEY_COUNT], int numNotesToSteal, NoteList &result) {

    }

    std::vector<std::unique_ptr<Strategy>> allStrategies() {
        auto result = std::vector<std::unique_ptr<Strategy>>();
        result.push_back(std::make_unique<Oldest>());
        return result;
    }

    Strategy::~Strategy() = default;
}