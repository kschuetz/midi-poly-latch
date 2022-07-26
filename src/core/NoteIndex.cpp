#include "NoteIndex.h"

int NoteIndex::insert(NoteNumber noteNumber) {
    if (!isValidNote(noteNumber)) return -1;
    NoteNumber insertionPoint = m_front;
    NoteNumber prev = NO_NOTE;
    int position = 0;
    while (isValidNote(insertionPoint)) {
        if (insertionPoint == noteNumber) return position;
        auto insertHere = insertionPoint > noteNumber;
        if (insertHere) break;
        else {
            position++;
            prev = insertionPoint;
            insertionPoint = forward[insertionPoint];
        }
    }
    NoteNumber next;
    if (isValidNote(prev)) {
        next = forward[prev];
        forward[prev] = noteNumber;
    } else {
        next = m_front;
        m_front = noteNumber;
    }
    forward[noteNumber] = next;
    reverse[noteNumber] = prev;
    if (isValidNote(next)) {
        reverse[next] = noteNumber;
    } else {
        m_back = noteNumber;
    }
    m_size += 1;
    return position;
}

bool NoteIndex::remove(NoteNumber noteNumber) {
    NoteNumber current = m_front;
    NoteNumber prev = NO_NOTE;
    while (isValidNote(current)) {
        if (current == noteNumber) {
            auto next = forward[current];
            if (isValidNote(prev)) {
                forward[prev] = next;
            } else {
                m_front = next;
            }
            if (isValidNote(next)) {
                reverse[next] = prev;
            } else {
                m_back = prev;
            }
            m_size -= 1;
            return true;
        } else {
            prev = current;
            current = forward[current];
        }
    }
    return false;
}

int NoteIndex::positionOf(NoteNumber noteNumber) const {
    NoteNumber current = m_front;
    int position = 0;
    while (isValidNote(current)) {
        if (current == noteNumber) return position;
        else {
            current = forward[current];
            ++position;
        }
    }
    return -1;
}

bool NoteIndex::contains(NoteNumber noteNumber) const {
    return positionOf(noteNumber) >= 0;
}

void NoteIndex::clear() {
    m_front = NO_NOTE;
    m_back = NO_NOTE;
    m_size = 0;
}

NoteIndexIterator NoteIndex::cbegin() const {
    return {m_front, forward};
}

NoteIndexIterator NoteIndex::cend() const {
    return {NO_NOTE, forward};
}

NoteIndexIterator NoteIndex::crbegin() const {
    return {m_back, reverse};
}

NoteIndexIterator NoteIndex::crend() const {
    return {NO_NOTE, reverse};
}

NoteNumber NoteIndex::atPosition(int index) const {
    auto current = m_front;
    while (isValidNote(current)) {
        if (index == 0) {
            return current;
        } else {
            --index;
            current = forward[current];
        }
    }
    return NO_NOTE;
}

NoteNumber NoteIndex::selectRandom(Rng &rng) const {
    if (m_size < 1) return NO_NOTE;
    else if (m_size == 1) return front();
    else {
        auto choice = rng.generate(m_size);
        return atPosition(choice);
    }
}

NoteIndexIterator::reference NoteIndexIterator::operator*() const {
    return const_cast<NoteNumber &>(m_current);
}

NoteIndexIterator::pointer NoteIndexIterator::operator->() {
    return &m_current;
}

NoteIndexIterator &NoteIndexIterator::operator++() {
    m_current = m_list[m_current];
    return *this;
}

const NoteIndexIterator NoteIndexIterator::operator++(int) {
    NoteIndexIterator tmp = {m_current, m_list};
    m_current = m_list[m_current];
    return tmp;
}

bool operator==(const NoteIndexIterator &a, const NoteIndexIterator &b) {
    return a.m_current == b.m_current;
}

bool operator!=(const NoteIndexIterator &a, const NoteIndexIterator &b) {
    return a.m_current != b.m_current;
}
