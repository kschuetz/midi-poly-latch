#pragma once

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include "core/NoteIndex.h"

struct IteratesForwardMatcher : Catch::Matchers::MatcherGenericBase {
    explicit IteratesForwardMatcher(std::vector<NoteNumber> &&expected) : m_expected(
            std::forward<std::vector<NoteNumber>>(expected)) {}

    bool match(const NoteIndex &noteIndex) const;

    std::string describe() const override;

private:
    std::vector<NoteNumber> const &m_expected;
};

struct IteratesReverseMatcher : Catch::Matchers::MatcherGenericBase {
    explicit IteratesReverseMatcher(std::vector<NoteNumber> &&expected) : m_expected(
            std::forward<std::vector<NoteNumber>>(expected)) {}

    bool match(const NoteIndex &noteIndex) const;

    std::string describe() const override;

private:
    std::vector<NoteNumber> const &m_expected;
};

auto IteratesForward(std::vector<NoteNumber> &&expected) -> IteratesForwardMatcher;

auto IteratesReverse(std::vector<NoteNumber> &&expected) -> IteratesReverseMatcher;