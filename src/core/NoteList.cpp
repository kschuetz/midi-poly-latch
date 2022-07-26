#include "NoteList.h"

void NoteList::add(NoteNumber noteNumber) {
    if (m_size < KEY_COUNT) {
        m_entries[m_size++] = noteNumber;
    }
}

void NoteList::clear() {
    m_size = 0;
}

NoteList::Iterator NoteList::begin() {
    return {&m_entries[0]};
}

NoteList::Iterator NoteList::end() {
    return {&m_entries[m_size]};
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
    for (int i = 0; i < m_size; i++) {
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
