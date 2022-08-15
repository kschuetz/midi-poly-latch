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
    TimePosition m_lastEventTime{0};

    [[nodiscard]] bool isActionNeededThisFrame(const juce::MidiBuffer &midiBuffer) const;

    [[nodiscard]] static bool doesBufferContainNoteMessages(const juce::MidiBuffer &midiBuffer);

    [[nodiscard]] bool doesAnyChannelHaveTooManyNotes(const juce::MidiBuffer &midiBuffer) const;

    void handleOmniActions(juce::MidiBuffer &output);

    void sendAllNotesOff(juce::MidiBuffer &output, int samplePosition);

    void sendAllNotesOff(juce::MidiBuffer &output, int samplePosition, int channelIndex);

    void sendOneNoteOff(juce::MidiBuffer &output, int samplePosition, int channelIndex);
};
