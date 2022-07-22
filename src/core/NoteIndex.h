#pragma once

#include <iterator>
#include <cstddef>
#include <functional>

#include "Consts.h"
#include "CommonTypes.h"

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
    [[nodiscard]] virtual int positionOf(NoteNumber noteNumber) const = 0;

    [[nodiscard]] virtual bool contains(NoteNumber noteNumber) const = 0;

    [[nodiscard]] virtual int getSize() const = 0;

    [[nodiscard]] virtual NoteIndexIterator begin() const = 0;

    [[nodiscard]] virtual NoteIndexIterator end() const = 0;

    [[nodiscard]] virtual NoteIndexIterator rbegin() const = 0;

    [[nodiscard]] virtual NoteIndexIterator rend() const = 0;
};

class NoteIndex final : public NoteIndexRead {
public:
    int insert(NoteNumber noteNumber, const std::function<bool(NoteNumber)> &shouldInsertBefore);

    bool remove(NoteNumber noteNumber);

    [[nodiscard]] int positionOf(NoteNumber noteNumber) const override;

    [[nodiscard]] bool contains(NoteNumber noteNumber) const override;

    void clear();

    [[nodiscard]] int getSize() const override {
        return size;
    }

    NoteIndexIterator begin() const override;

    NoteIndexIterator end() const override;

    NoteIndexIterator rbegin() const override;

    NoteIndexIterator rend() const override;

private:
    int size = 0;
    NoteNumber forward[KEY_COUNT]{};
    NoteNumber reverse[KEY_COUNT]{};
    NoteNumber front = NO_NOTE;
    NoteNumber back = NO_NOTE;
};
