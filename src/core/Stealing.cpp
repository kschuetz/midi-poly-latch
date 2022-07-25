#include "Stealing.h"

namespace Stealing {

    enum class PreferAge {
        Older, Newer
    };

    NoteNumber selectFirst(const NoteIndexIterator &begin, const NoteIndexIterator &end);

    NoteNumber selectByAge(const Strategy &strategy, const State &state, Rng &rng, PreferAge preferAge);

    NoteNumber selectNoteToSteal(const Strategy &strategy, const State &state, Rng &rng, NoteNumber userNotePlayed) {
        const NoteIndexRead &indexByPitch = state.indexByPitch();
        switch (strategy.primary) {
            case PrimaryStrategy::Oldest:
                return selectByAge(strategy, state, rng, PreferAge::Older);
            case PrimaryStrategy::Newest:
                return selectByAge(strategy, state, rng, PreferAge::Newer);
            case PrimaryStrategy::Lowest:
                return indexByPitch.front();
            case PrimaryStrategy::Highest:
                return indexByPitch.back();
            case PrimaryStrategy::Closest:
                break;
            case PrimaryStrategy::MostDistant:
                break;
            case PrimaryStrategy::LeastDistant:
                break;
            case PrimaryStrategy::MostCentral:
                break;
            case PrimaryStrategy::Random:
                break;
            case PrimaryStrategy::RandomInner:
                break;
            case PrimaryStrategy::RandomOuter:
                break;
        }
        return NO_NOTE;
    }


    NoteNumber selectByAge(const Strategy &strategy, const State &state, Rng &rng, PreferAge preferAge) {
        auto &index = state.indexByPitch();
        auto it = index.cbegin();
        if (it == index.cend()) return NO_NOTE;
        auto current = *it;
        auto currentTimestamp = state.getNoteState(current).startedPlaying;
        auto best = current;
        auto bestTimestamp = currentTimestamp;
        auto ties = 0;
        while (++it != index.cend()) {
            currentTimestamp = state.getNoteState(current).startedPlaying;
            auto compared = currentTimestamp.compare(bestTimestamp);
            if (compared == 0) {
                ties++;
            } else {
                bool isBetter = (preferAge == PreferAge::Older && compared < 0) ||
                                (preferAge == PreferAge::Newer && compared > 0);
                if (isBetter) {
                    best = current;
                    bestTimestamp = currentTimestamp;
                    ties = 0;
                }
            }
        }
        if (ties <= 0) {
            return best;
        } else {
            return NO_NOTE;
            // TODO: apply secondary strategy
        }
    }

    NoteNumber selectFirst(const NoteIndexIterator &begin, const NoteIndexIterator &end) {
        if (begin == end) return NO_NOTE;
        else return *begin;
    }

}