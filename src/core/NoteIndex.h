#pragma once

#include <iterator>
#include <cstddef>
#include <functional>

#include "Consts.h"
#include "CommonTypes.h"
#include "Rng.h"

struct NoteIndexIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = NoteNumber;
    using pointer = NoteNumber *;
    using reference = NoteNumber &;

    NoteIndexIterator(NoteNumber current, const NoteNumber (&list)[KEY_COUNT]) : m_current(current), m_list(list) {}

    reference operator*() const;

    pointer operator->();

    NoteIndexIterator &operator++();

    const NoteIndexIterator operator++(int);

    friend bool operator==(const NoteIndexIterator &a, const NoteIndexIterator &b);

    friend bool operator!=(const NoteIndexIterator &a, const NoteIndexIterator &b);

private:
    NoteNumber m_current;
    const NoteNumber (&m_list)[KEY_COUNT];
};

class NoteIndexRead {
public:
    [[nodiscard]] virtual NoteNumber front() const = 0;

    [[nodiscard]] virtual NoteNumber back() const = 0;

    [[nodiscard]] virtual int positionOf(NoteNumber noteNumber) const = 0;

    [[nodiscard]] virtual bool contains(NoteNumber noteNumber) const = 0;

    [[nodiscard]] virtual int size() const = 0;

    [[nodiscard]] virtual NoteNumber selectRandom(Rng &rng) const = 0;

    [[nodiscard]] virtual NoteIndexIterator cbegin() const = 0;

    [[nodiscard]] virtual NoteIndexIterator cend() const = 0;

    [[nodiscard]] virtual NoteIndexIterator crbegin() const = 0;

    [[nodiscard]] virtual NoteIndexIterator crend() const = 0;
};

class NoteIndex final : public NoteIndexRead {
public:
    int insert(NoteNumber noteNumber);

    bool remove(NoteNumber noteNumber);

    [[nodiscard]] NoteNumber front() const override {
        return m_front;
    }

    [[nodiscard]] NoteNumber back() const override {
        return m_back;
    }

    [[nodiscard]] NoteNumber atPosition(int index) const;

    [[nodiscard]] int positionOf(NoteNumber noteNumber) const override;

    [[nodiscard]] bool contains(NoteNumber noteNumber) const override;

    void clear();

    [[nodiscard]] int size() const override {
        return m_size;
    }

    [[nodiscard]] NoteNumber selectRandom(Rng &rng) const override;

    NoteIndexIterator cbegin() const override;

    NoteIndexIterator cend() const override;

    NoteIndexIterator crbegin() const override;

    NoteIndexIterator crend() const override;

private:
    int m_size = 0;
    NoteNumber forward[KEY_COUNT]{};
    NoteNumber reverse[KEY_COUNT]{};
    NoteNumber m_front = NO_NOTE;
    NoteNumber m_back = NO_NOTE;
};
