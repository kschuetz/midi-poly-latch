
#include "Engine.h"
#include "Stealing.h"

void Engine::process(juce::MidiBuffer &midiBuffer) {
    if (!isActionNeededThisFrame(midiBuffer)) return;
    auto &output = m_processedBuffer;
    output.clear();

    m_timePosition.newFrame();

    handleOmniActions(output);

    for (auto iter: midiBuffer) {
        const juce::MidiMessage &message = iter.getMessage();
        int samplePosition = iter.samplePosition;
        if (message.isNoteOn(true)) {
            handleNoteOn(output, message, samplePosition);
        } else if (message.isNoteOff(true)) {
            handleNoteOff(output, message, samplePosition);
        } else {
            output.addEvent(message, samplePosition);
        }
    }

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
        const NoteList &toggledNotes = m_omniActions.toggledNotes();
        for (auto it = toggledNotes.cbegin(); it != toggledNotes.cend(); ++it) {
            handleOmniToggledNote(output, *it);
        }
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
            writeNoteOff(output, samplePosition, channelIndex, note, MaxVelocity);
            note = *(++it);
        }
    }
    channel.clear();
}

void Engine::sendOneNoteOff(juce::MidiBuffer &output, int samplePosition, int channelIndex) {

}

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

void Engine::handleOmniToggledNote(juce::MidiBuffer &output, NoteNumber note) {
    if (isNotePlayingOnAnyChannel(note)) {
        omniNoteOff(output, note);
    } else {
        writeNoteOn(output, 0, 0, note, MaxVelocity);
    }
}

void Engine::omniNoteOff(juce::MidiBuffer &output, NoteNumber note) {
    for (int channelIndex = 0; channelIndex < CHANNEL_COUNT; channelIndex++) {
        auto channel = m_channels[channelIndex];
        if (channel.noteState(note).isPlaying()) {
            writeNoteOff(output, 0, channelIndex, note, MaxVelocity);  // TODO: velocity
        }
    }
}

void Engine::handleNoteOn(juce::MidiBuffer &output, const juce::MidiMessage &message, int samplePosition) {
    auto note = static_cast<NoteNumber>(message.getNoteNumber());
    auto channelIndex = message.getChannel() - 1;
    auto channel = m_channels[channelIndex];
    NoteStatus status = channel.noteState(note).status;
    if (status == NoteStatus::Playing) {
        if (m_parameters.releaseTrigger() == ReleaseTrigger::SameKeyDown) {
            writeNoteOff(output, samplePosition, channelIndex, note, MaxVelocity);
        } else {
            auto t = m_timePosition.update(samplePosition);
            channel.beginRelease(note, MaxVelocity, t);
        }
    } else if (status == NoteStatus::NotPlaying) {
        while (channel.playingCount() >= m_parameters.maxVoices()) {
            NoteNumber noteToSteal = selectNoteToSteal(m_parameters.stealingStrategy(), channel, m_rng, note);
            if (isValidNote(noteToSteal)) {
                writeNoteOff(output, samplePosition, channelIndex, noteToSteal, MaxVelocity);
            } else {
                // safety feature
                break;
            }
        }
        if (channel.playingCount() < m_parameters.maxVoices()) {
            writeNoteOn(output, samplePosition, channelIndex, note, Velocity(message.getVelocity()));
        }
    }
}

void Engine::handleNoteOff(juce::MidiBuffer &output, const juce::MidiMessage &message, int samplePosition) {
    auto note = static_cast<NoteNumber>(message.getNoteNumber());
    auto channelIndex = message.getChannel() - 1;
    auto channel = m_channels[channelIndex];
    NoteStatus status = channel.noteState(note).status;
    if (status == NoteStatus::Playing) {
        // discard
    } else if (status == NoteStatus::ReadyToRelease) {
        writeNoteOff(output, samplePosition, channelIndex, note, MaxVelocity);
    } else {
        // not playing, but no reason not to let it through
        output.addEvent(message, samplePosition);
    }
}

void
Engine::writeNoteOn(juce::MidiBuffer &output, int samplePosition, int channelIndex, NoteNumber note,
                    Velocity velocity) {
    auto t = m_timePosition.update(samplePosition);
    m_channels[channelIndex].activateNote(note, velocity, t);
    auto message = juce::MidiMessage::noteOn(channelIndex + 1, note, velocity.value());
    output.addEvent(message, samplePosition);
}

void
Engine::writeNoteOff(juce::MidiBuffer &output, int samplePosition, int channelIndex, NoteNumber note,
                     Velocity velocity) {
    m_channels[channelIndex].deactivateNote(note);
    auto message = juce::MidiMessage::noteOff(channelIndex + 1, note, velocity.value());
    output.addEvent(message, samplePosition);
}

bool Engine::isNotePlayingOnAnyChannel(NoteNumber noteNumber) const {
    for (const auto &m_channel: m_channels) {
        if (m_channel.noteState(noteNumber).isPlaying()) {
            return true;
        }
    }
    return false;
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