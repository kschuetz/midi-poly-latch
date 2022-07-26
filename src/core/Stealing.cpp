#include "Stealing.h"

using namespace Stealing;

template<typename C = std::less<>>
NoteNumber selectByAge(const Strategy &strategy, const State &state, Rng &rng, C cmp = C{});

NoteNumber selectRandomInner(const NoteIndex &noteIndex, Rng &rng);

NoteNumber selectRandomOuter(const NoteIndex &noteIndex, Rng &rng);

NoteNumber applySecondaryStrategy(SecondaryStrategy secondaryStrategy, Rng &rng, const NoteList &candidates);

NoteNumber selectNoteToSteal(const Strategy &strategy, const State &state, Rng &rng, NoteNumber userNotePlayed) {
    const NoteIndex &indexByPitch = state.indexByPitch();
    switch (strategy.primary) {
        case PrimaryStrategy::Oldest:
            return selectByAge(strategy, state, rng, std::less());
        case PrimaryStrategy::Newest:
            return selectByAge(strategy, state, rng, std::greater());
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

NoteNumber breakTiesForAge(const Strategy &strategy, const State &state, Rng &rng, const NoteIndex &index,
                           const Timestamp &bestTimestamp) {
    NoteList candidates;
    auto it = index.cbegin();
    auto note = *it;
    while (isValidNote(note)) {
        if (state.getNoteState(note).startedPlaying == bestTimestamp) {
            candidates.add(note);
        }
        note = *(++it);
    }
    return applySecondaryStrategy(strategy.secondary, rng, candidates);
}

template<typename C>
NoteNumber selectByAge(const Strategy &strategy, const State &state, Rng &rng, C cmp) {
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
        auto compareScore = currentTimestamp.compare(bestTimestamp);
        if (compareScore == 0) {
            ties++;
        } else {
            bool isBetter = cmp(compareScore, 0);
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
        return breakTiesForAge(strategy, state, rng, index, bestTimestamp);
    }
}

NoteNumber selectRandomOuter(const NoteIndex &noteIndex, Rng &rng) {
    if (noteIndex.size() <= 2) {
        return noteIndex.selectRandom(rng);
    } else {
        NoteList candidates;
        candidates.add(noteIndex.front());
        candidates.add(noteIndex.back());
        return candidates.selectRandom(rng);
    }
}

NoteNumber selectRandomInner(const NoteIndex &noteIndex, Rng &rng) {
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
    if (candidates.size() < 1) return NO_NOTE;
    else if (candidates.size() == 1) return candidates.atPosition(0);
    else
        switch (secondaryStrategy) {
            case SecondaryStrategy::StealRandom:
                return candidates.selectRandom(rng);
            case SecondaryStrategy::StealLower:
                return candidates.best(std::less());
            case SecondaryStrategy::StealHigher:
                return candidates.best(std::greater());
        }
}
