

#include "NoteState.h"

bool NoteState::isPlaying() const {
    return status == NoteStatus::Playing || status == NoteStatus::ReadyToRelease;
}
