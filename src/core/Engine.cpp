
#include "Engine.h"

void Engine::process(juce::MidiBuffer &midiBuffer) {
    if (!isActionNeededThisFrame(midiBuffer)) return;
    auto &output = m_processedBuffer;
    output.clear();

    Timestamp currentTimestamp = m_lastEventTime;
    int sampleNumberOfCurrentTimestamp = 0;

    handleOmniActions(output);

//    for (auto iter: midiBuffer) {
//        const juce::MidiMessage &message = iter.getMessage();
//        int samplePosition = iter.samplePosition
//        if (message.isNoteOn(true)) {
//            auto note = static_cast<NoteNumber>(message.getNoteNumber());
//            auto channel = message.getChannel() - 1;
//
//        }
//    }

    midiBuffer.swapWith(output);
}

void Engine::handleOmniActions(juce::MidiBuffer &output) {
    if (m_omniActions.wasAllNotesOffTriggered()) {
        sendAllNotesOff(output, 0);
    } else {
        if (m_omniActions.wasOneNoteOffTriggered()) {
            for (auto channelIdx = 0; channelIdx < CHANNEL_COUNT; channelIdx++) {
                sendOneNoteOff(output, 0, channelIdx);
            }
        }
        // TODO: list off here
    }
}

void Engine::sendAllNotesOff(juce::MidiBuffer &output, int samplePosition) {
    for (auto channelIdx = 0; channelIdx < CHANNEL_COUNT; channelIdx++) {
        sendAllNotesOff(output, samplePosition, channelIdx);
    }
}

void Engine::sendAllNotesOff(juce::MidiBuffer &output, int samplePosition, int channelIndex) {
    auto &channel = m_channels[channelIndex];
    if (channel.hasActiveNotes()) {
        auto it = channel.noteIndex().cbegin();
        auto note = *it;
        while (isValidNote(note)) {
            // TODO: release velocity?
            auto message = juce::MidiMessage::noteOff(channelIndex + 1, note, 1.0f);
            output.addEvent(message, samplePosition);
            note = *(++it);
        }
    }
    channel.clear();
}

void Engine::sendOneNoteOff(juce::MidiBuffer &output, int samplePosition, int channelIndex) {

}

/*
 * NoteNumber breakTiesForAge(const Strategy &strategy, const ChannelState &state, Rng &rng, const NoteIndex &index,
                           const Timestamp &bestTimestamp) {
    NoteList candidates;
    auto it = index.cbegin();
    auto note = *it;
    while (isValidNote(note)) {
        if (state.noteState(note).startedPlaying == bestTimestamp) {
            candidates.add(note);
        }
        note = *(++it);
    }
    return applySecondaryStrategy(strategy.secondary, rng, candidates);
}
 */

bool Engine::isActionNeededThisFrame(const juce::MidiBuffer &midiBuffer) const {
    return (m_omniActions.isBufferWriteNeeded() ||
            doesAnyChannelHaveTooManyNotes(midiBuffer) ||
            doesBufferContainNoteMessages(midiBuffer));
}

bool Engine::doesAnyChannelHaveTooManyNotes(const juce::MidiBuffer &midiBuffer) const {
    int maxVoices = m_parameters.maxVoices();
    for (const auto &m_channel: m_channels) {
        if (m_channel.playingCount() > maxVoices) {
            return true;
        }
    }
    return false;
}

bool Engine::doesBufferContainNoteMessages(const juce::MidiBuffer &midiBuffer) {
    for (auto iter: midiBuffer) {
        const juce::MidiMessage &message = iter.getMessage();
        if (message.isNoteOnOrOff()) return true;
    }
    return false;
}

void Engine::triggerAllNotesOffNextFrame() {
    m_omniActions.triggerAllNotesOff();
}

void Engine::triggerOneNoteOffNextFrame() {
    m_omniActions.triggerOneNoteOff();
}

/**
 * - Note off when note state is NotPlaying:
 *      Discard
 *
 * - Any note-on when state is not playing, and current playing count >= maxVoices:
 *      1. Find note to steal - send note off for that note
 *      2. Send new note-on right after note-off for stolen note
 *
 * - Note on for special key: override
 *      Enable override
 *      Discard
 *
 * - Note off for special key: override
 *      Disable override
 *      Discard
 *
 * - Note on for special key: all notes off
 *      Discard
 *      Send note off for all active keys
 *      Ignore all
 *
 * - Note off for special key: all notes off
 *      Discard
 *      Disable ignore all
 *
 * - Note on for special key: one note off
 *      Discard
 *      Find note to steal - send note off for that note
 *
 * - Note off for special key: one note off
 *      Discard
 *
 * - Any note-on or note-off of a special key (override, all notes off, one note off)
 *      Discard
 *
 * - Any note-on when key is already down
 *      Discard
 *      if release trigger is SameKeyDown, send note off
 *
 * - Any note-off when status is "playing"
 *      Discard
 */