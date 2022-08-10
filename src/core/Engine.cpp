
#include "Engine.h"

void Engine::process(juce::MidiBuffer &midiBuffer) {

}

void Engine::triggerAllNotesOffNextFrame() {
    m_allNotesOffTriggered = true;
}

void Engine::triggerOneNoteOffNextFrame() {
    m_oneNoteOffTriggered = true;
}
