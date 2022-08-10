#pragma once

#include "Parameters.h"

class Engine {
private:
    Parameters m_parameters;
    ChannelState m_state;
    Rng m_rng;
};
