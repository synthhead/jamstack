#include "voice_manager.h"

namespace jamstack
{
void VoiceManager::NotaOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  pressed_keys_.NoteOn(note, velocity);
}

void VoiceManager::NotaOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  pressed_keys_.NoteOff(note);
}

} // namespace jamstack
