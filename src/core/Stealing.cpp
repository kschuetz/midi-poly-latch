#include "Stealing.h"

using namespace Stealing;

enum class PreferAge {
    Older, Newer
};

NoteNumber selectByAge(const Strategy &strategy, const State &state, Rng &rng, PreferAge preferAge);

NoteNumber selectRandomInner(const NoteIndexRead &noteIndex, Rng &rng);

NoteNumber selectRandomOuter(const NoteIndexRead &noteIndex, Rng &rng);

NoteNumber applySecondaryStrategy(SecondaryStrategy secondaryStrategy, Rng &rng, const NoteList &candidates);

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
        case PrimaryStrategy::MostDistant:
            break;
        case PrimaryStrategy::LeastDistant:
            break;
        case PrimaryStrategy::MostCentral:
            break;
        case PrimaryStrategy::Random:
            return indexByPitch.selectRandom(rng);
        case PrimaryStrategy::RandomInner:
            return selectRandomInner(indexByPitch, rng);
        case PrimaryStrategy::RandomOuter:
            return selectRandomOuter(indexByPitch, rng);
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
        NoteList candidates;
        auto it2 = index.cbegin();
        auto note = *it2;
        while (isValidNote(note)) {
            if (state.getNoteState(note).startedPlaying == bestTimestamp) {
                candidates.add(note);
            }
            note = *(++it2);
        }
        return applySecondaryStrategy(strategy.secondary, rng, candidates);
    }
}

NoteNumber selectRandomOuter(const NoteIndexRead &noteIndex, Rng &rng) {
    if (noteIndex.size() <= 2) {
        return noteIndex.selectRandom(rng);
    } else {
        NoteList candidates;
        candidates.add(noteIndex.front());
        candidates.add(noteIndex.back());
        return candidates.selectRandom(rng);
    }
}

NoteNumber selectRandomInner(const NoteIndexRead &noteIndex, Rng &rng) {
    if (noteIndex.size() <= 2) {
        return noteIndex.selectRandom(rng);
    } else {
        NoteList candidates;
        auto it = noteIndex.cbegin();
        while (it != noteIndex.cend()) {
            auto note = *(it++);
            if (note == noteIndex.back()) break;
            if (note != noteIndex.front()) {
                candidates.add(note);
            }
        }
        return candidates.selectRandom(rng);
    }
}

NoteNumber applySecondaryStrategy(SecondaryStrategy secondaryStrategy, Rng &rng, const NoteList &candidates) {
    return NO_NOTE;
}
