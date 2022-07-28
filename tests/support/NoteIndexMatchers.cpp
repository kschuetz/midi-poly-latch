#include "NoteIndexMatchers.h"

bool IteratesForwardMatcher::match(const NoteIndex &noteIndex) const {
    auto it = noteIndex.cbegin();
    auto e = m_expected.cbegin();
    for (; it != noteIndex.cend() && e != m_expected.cend(); ++it, ++e) {
        if (*it != *e) return false;
    }
    return it == noteIndex.cend() && e == m_expected.cend();
}

std::string IteratesForwardMatcher::describe() const {
    return "Iterates forward";
}

bool IteratesReverseMatcher::match(const NoteIndex &noteIndex) const {
    auto it = noteIndex.crbegin();
    auto e = m_expected.cbegin();
    for (; it != noteIndex.crend() && e != m_expected.cend(); ++it, ++e) {
        if (*it != *e) return false;
    }
    return it == noteIndex.crend() && e == m_expected.cend();
}

std::string IteratesReverseMatcher::describe() const {
    return "Iterates reverse";
}

auto IteratesForward(std::vector <NoteNumber> &&expected) -> IteratesForwardMatcher {
    return IteratesForwardMatcher{std::forward < std::vector < NoteNumber >> (expected)};
}

auto IteratesReverse(std::vector <NoteNumber> &&expected) -> IteratesReverseMatcher {
    return IteratesReverseMatcher{std::forward < std::vector < NoteNumber >> (expected)};
}
