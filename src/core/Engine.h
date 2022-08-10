#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Consts.h"
#include "Parameters.h"

class Engine final {
    void process(juce::MidiBuffer &midiBuffer);

    void triggerAllNotesOffNextFrame();

    void triggerOneNoteOffNextFrame();

private:
    Parameters m_parameters;
    ChannelState m_channels[CHANNEL_COUNT];
    Rng m_rng;
    juce::MidiBuffer m_processedBuffer;
    bool m_allNotesOffTriggered;
    bool m_oneNoteOffTriggered;
};
