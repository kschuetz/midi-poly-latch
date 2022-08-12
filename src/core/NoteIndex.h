#pragma once

#include <iterator>
#include <cstddef>
#include <functional>

#include "Consts.h"
#include "CommonTypes.h"
#include "Rng.h"
#include "NoteList.h"

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

class NoteIndex final {
public:
    int insert(NoteNumber noteNumber);

    bool remove(NoteNumber noteNumber);

    [[nodiscard]] NoteNumber front() const {
        return m_front;
    }

    [[nodiscard]] NoteNumber back() const {
        return m_back;
    }

    [[nodiscard]] NoteNumber atPosition(int index) const;

    [[nodiscard]] int positionOf(NoteNumber noteNumber) const;

    [[nodiscard]] bool contains(NoteNumber noteNumber) const;

    void clear();

    [[nodiscard]] int size() const {
        return m_size;
    }

    [[nodiscard]] bool isEmpty() const {
        return m_size == 0;
    }

    [[nodiscard]] NoteNumber selectRandom(Rng &rng) const;

    void addAllToNoteList(NoteList &dest) const;

    NoteIndexIterator cbegin() const;

    NoteIndexIterator cend() const;

    NoteIndexIterator crbegin() const;

    NoteIndexIterator crend() const;

private:
    int m_size = 0;
    NoteNumber forward[KEY_COUNT]{};
    NoteNumber reverse[KEY_COUNT]{};
    NoteNumber m_front = NO_NOTE;
    NoteNumber m_back = NO_NOTE;
};
