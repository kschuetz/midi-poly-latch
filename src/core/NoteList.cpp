#include "NoteList.h"

void NoteList::add(NoteNumber noteNumber) {
    if (size < KEY_COUNT && !contains(noteNumber)) {
        entries[size++] = noteNumber;
    }
}

void NoteList::clear() {
    size = 0;
}

[[nodiscard]]
int NoteList::getSize() const {
    return size;
}

NoteList::Iterator NoteList::begin() {
    return {&entries[0]};
}

NoteList::Iterator NoteList::end() {
    return {&entries[size]};
}

NoteList::Iterator::reference NoteList::Iterator::operator*() const {
    return *m_ptr;
}

NoteList::Iterator::pointer NoteList::Iterator::operator->() {
    return m_ptr;
}

NoteList::Iterator &NoteList::Iterator::operator++() {
    m_ptr++;
    return *this;
}

const NoteList::Iterator NoteList::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool operator==(const NoteList::Iterator &a, const NoteList::Iterator &b) {
    return a.m_ptr == b.m_ptr;
}

bool operator!=(const NoteList::Iterator &a, const NoteList::Iterator &b) {
    return a.m_ptr != b.m_ptr;
}

bool NoteList::contains(NoteNumber noteNumber) const {
    for (int i = 0; i < size; i++) {
        if (entries[i] == noteNumber) return true;
    }
    return false;
}

int NoteList::remove(NoteNumber noteNumber) {
    int removedCount = 0;
    int src = 0;
    int dest = 0;
    while (src < size) {
        auto entry = entries[src];
        if (entry == noteNumber) {
            removedCount++;
        } else {
            if (dest != src) {
                entries[dest] = entry;
            }
            dest++;
        }
        src++;
    }
    size -= removedCount;
    return removedCount;
}
