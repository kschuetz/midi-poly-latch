#pragma once

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

#include "Consts.h"
#include "CommonTypes.h"

class NoteList final {
public:
    void add(NoteNumber noteNumber);

    int remove(NoteNumber noteNumber);

    [[nodiscard]] bool contains(NoteNumber noteNumber) const;

    void clear();

    [[nodiscard]] int getSize() const;

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = NoteNumber;
        using pointer = NoteNumber *;
        using reference = NoteNumber &;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const;

        pointer operator->();

        Iterator &operator++();

        const Iterator operator++(int);

        friend bool operator==(const Iterator &a, const Iterator &b);

        friend bool operator!=(const Iterator &a, const Iterator &b);

    private:
        pointer m_ptr;
    };

    Iterator begin();

    Iterator end();

private:
    int size = 0;
    NoteNumber entries[KEY_COUNT]{};
};
