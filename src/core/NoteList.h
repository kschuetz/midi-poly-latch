#pragma once

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

#include "Consts.h"
#include "CommonTypes.h"
#include "Rng.h"

using NoteListEntries = NoteNumber[KEY_COUNT];

struct NoteListIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = NoteNumber;
    using pointer = const NoteNumber *;
    using reference = NoteNumber &;

    NoteListIterator(pointer ptr) : m_ptr(ptr) {}

    NoteNumber operator*() const;

    pointer operator->();

    NoteListIterator &operator++();

    const NoteListIterator operator++(int);

    friend bool operator==(const NoteListIterator &a, const NoteListIterator &b);

    friend bool operator!=(const NoteListIterator &a, const NoteListIterator &b);

private:
    pointer m_ptr;
};

/**
 * List of notes stored in an array. Unordered, with a maximum size of KEY_COUNT.
 * Duplicates are not removed.
 */
class NoteList final {
public:
    void add(NoteNumber noteNumber);

    [[nodiscard]] bool contains(NoteNumber noteNumber) const;

    void clear();

    [[nodiscard]] NoteNumber atPosition(int index) const;

    [[nodiscard]] int size() const {
        return m_size;
    }

    [[nodiscard]] bool isEmpty() const {
        return m_size <= 0;
    }

    [[nodiscard]] NoteListEntries &entries() {
        return m_entries;
    }

    template<typename C = std::less<>>
    [[nodiscard]] NoteNumber best(C cmp = C{}) const {
        if (m_size < 1) return NO_NOTE;
        else {
            auto best = m_entries[0];
            for (int i = 1; i < m_size; i++) {
                auto entry = m_entries[i];
                if (cmp(entry, best)) best = entry;
            }
            return best;
        }
    }

    [[nodiscard]] NoteNumber selectRandom(Rng &rng) const;

//    struct Iterator {
//        using iterator_category = std::forward_iterator_tag;
//        using difference_type = std::ptrdiff_t;
//        using value_type = NoteNumber;
//        using pointer = NoteNumber *;
//        using reference = NoteNumber &;
//
//        Iterator(pointer ptr) : m_ptr(ptr) {}
//
//        reference operator*() const;
//
//        pointer operator->();
//
//        Iterator &operator++();
//
//        const Iterator operator++(int);
//
//        friend bool operator==(const Iterator &a, const Iterator &b);
//
//        friend bool operator!=(const Iterator &a, const Iterator &b);
//
//    private:
//        pointer m_ptr;
//    };

    [[nodiscard]] NoteListIterator cbegin() const;

    [[nodiscard]] NoteListIterator cend() const;

private:
    int m_size = 0;
    NoteListEntries m_entries{};
};
