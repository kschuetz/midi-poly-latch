#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Consts.h"
#include "Parameters.h"
#include "OmniActions.h"

class Engine final {
public:
    void process(juce::MidiBuffer &midiBuffer);

    void triggerAllNotesOffNextFrame();

    void triggerOneNoteOffNextFrame();

private:
    Parameters m_parameters;
    ChannelState m_channels[CHANNEL_COUNT];
    Rng m_rng;
    juce::MidiBuffer m_processedBuffer;
    OmniActions m_omniActions;
    Timestamp m_lastEventTime{0};

    bool isBufferWriteNeeded(juce::MidiBuffer &midiBuffer);

    bool doesAnyChannelHaveTooManyNotes(juce::MidiBuffer &midiBuffer);

    bool doesAnyNoteNeedToBeTurnedOff(juce::MidiBuffer &midiBuffer);

    void processReadOnly(juce::MidiBuffer &midiBuffer);

    void processWrite(juce::MidiBuffer &midiBuffer);
};
