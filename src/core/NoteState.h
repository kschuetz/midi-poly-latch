#pragma once

#include "CommonTypes.h"

struct NoteState {
    NoteStatus status = NoteStatus::NotPlaying;
    Timestamp startedPlaying = Timestamp(0);
    Timestamp beganRelease = Timestamp(0);
    Velocity initialDownVelocity = Velocity(0);
    Velocity initialUpVelocity = Velocity(0);
    Velocity releaseVelocity = Velocity(0);
};
