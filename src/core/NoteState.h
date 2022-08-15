#pragma once

#include "CommonTypes.h"
#include "TimePosition.h"

struct NoteState {
    NoteStatus status = NoteStatus::NotPlaying;
    TimePosition startedPlaying = TimePosition(0);
    TimePosition beganRelease = TimePosition(0);
    Velocity initialDownVelocity = Velocity(0);
    Velocity initialUpVelocity = Velocity(0);
    Velocity releaseVelocity = Velocity(0);
};
