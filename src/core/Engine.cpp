
#include "Engine.h"

void Engine::process(juce::MidiBuffer &midiBuffer) {
    if (isBufferWriteNeeded(midiBuffer)) {
        processWrite(midiBuffer);
    } else {
        processReadOnly(midiBuffer);
    }
}

void Engine::processReadOnly(juce::MidiBuffer &midiBuffer) {

}

void Engine::processWrite(juce::MidiBuffer &midiBuffer) {

}

bool Engine::isBufferWriteNeeded(juce::MidiBuffer &midiBuffer) {
    return (m_omniActions.isBufferWriteNeeded() ||
            doesAnyChannelHaveTooManyNotes(midiBuffer) ||
            doesAnyNoteNeedToBeTurnedOff(midiBuffer));
}

bool Engine::doesAnyChannelHaveTooManyNotes(juce::MidiBuffer &midiBuffer) {
    int maxVoices = m_parameters.maxVoices();
    for (const auto &m_channel: m_channels) {
        if (m_channel.playingCount() > maxVoices) {
            return true;
        }
    }
    return false;
}

bool Engine::doesAnyNoteNeedToBeTurnedOff(juce::MidiBuffer &midiBuffer) {
    if (m_parameters.releaseTrigger() == ReleaseTrigger::SameKeyDown) {
        for (auto iter: midiBuffer) {
            const juce::MidiMessage &message = iter.getMessage();
            if (message.isNoteOn(true)) {
                auto note = static_cast<NoteNumber>(message.getNoteNumber());
                auto channel = message.getChannel() - 1;
                if (m_channels[channel].noteStatus(note) == NoteStatus::Playing) {
                    return true;
                }
            }
        }
        return false;
    } else {
        for (auto iter: midiBuffer) {
            const juce::MidiMessage &message = iter.getMessage();
            if (message.isNoteOff(true)) {
                auto note = static_cast<NoteNumber>(message.getNoteNumber());
                auto channel = message.getChannel() - 1;
                if (m_channels[channel].noteStatus(note) == NoteStatus::ReadyToRelease) {
                    return true;
                }
            }
        }
        return false;
    }
}

void Engine::triggerAllNotesOffNextFrame() {
    m_omniActions.triggerAllNotesOff();
}

void Engine::triggerOneNoteOffNextFrame() {
    m_omniActions.triggerOneNoteOff();
}
