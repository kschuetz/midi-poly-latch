#pragma once

#include <functional>

#include "Consts.h"
#include "CommonTypes.h"

class NoteIndex final {
    int insert(NoteNumber noteNumber, const std::function<bool(NoteNumber)> &shouldInsertBefore);

    bool remove(NoteNumber noteNumber);

    [[nodiscard]] int positionOf(NoteNumber noteNumber) const;

    [[nodiscard]] bool contains(NoteNumber noteNumber) const;

    void clear();

    [[nodiscard]] int getSize() const {
        return size;
    }

private:
    int size = 0;
    NoteNumber forward[KEY_COUNT]{};
    NoteNumber reverse[KEY_COUNT]{};
    NoteNumber front = NO_NOTE;
    NoteNumber back = NO_NOTE;
};
