#include "NoteList.h"

void NoteList::add(NoteNumber noteNumber) {
    if (m_size < KEY_COUNT) {
        m_entries[m_size++] = noteNumber;
    }
}

void NoteList::clear() {
    m_size = 0;
}

NoteListIterator NoteList::cbegin() {
    return {&m_entries[0]};
}

NoteListIterator NoteList::cend() {
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

template<typename C>
NoteNumber NoteList::best(C cmp) const {
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

NoteNumber &NoteListIterator::operator*() const {
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
