#include "NoteList.h"

void NoteList::add(NoteNumber noteNumber) {
    if (m_size < KEY_COUNT) {
        m_entries[m_size++] = noteNumber;
    }
}

void NoteList::clear() {
    m_size = 0;
}

NoteListIterator NoteList::cbegin() const {
    return {&m_entries[0]};
}

NoteListIterator NoteList::cend() const {
    return {&m_entries[m_size]};
}

bool NoteList::contains(NoteNumber noteNumber) const {
    for (auto i = 0; i < m_size; i++) {
        if (m_entries[i] == noteNumber) return true;
    }
    return false;
}

NoteNumber NoteList::atPosition(int index) const {
    if (index >= 0 && index < m_size) {
        return m_entries[index];
    } else return NO_NOTE;
}

NoteNumber NoteList::selectRandom(Rng &rng) const {
    if (m_size < 1) return NO_NOTE;
    else if (m_size == 1) return m_entries[0];
    else {
        auto choice = rng.generate(m_size);
        return m_entries[choice];
    }
}

NoteNumber NoteListIterator::operator*() const {
    return *m_ptr;
}

NoteListIterator::pointer NoteListIterator::operator->() {
    return m_ptr;
}

NoteListIterator &NoteListIterator::operator++() {
    m_ptr++;
    return *this;
}

const NoteListIterator NoteListIterator::operator++(int) {
    NoteListIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool operator==(const NoteListIterator &a, const NoteListIterator &b) {
    return a.m_ptr == b.m_ptr;
}

bool operator!=(const NoteListIterator &a, const NoteListIterator &b) {
    return a.m_ptr != b.m_ptr;
}
