#ifndef GROK_VOICE_MANAGER
#define GROK_VOICE_MANAGER

// #include "Jamstack.h"
#include "jamstackcommons.h"
#include "note_stack.h"

namespace jamstack
{
//
class VoiceManager
{
public:
  VoiceManager() {}
  ~VoiceManager() {}

  void    Init() { pressed_keys_.Init(); }
  void    NotaOn(uint8_t channel, uint8_t note, uint8_t velocity);
  void    NotaOff(uint8_t channel, uint8_t note, uint8_t velocity);
  uint8_t size() { return pressed_keys_.size(); }
  uint8_t most_recent_note() { return pressed_keys_.most_recent_note().note; }
  // uint8_t most_recent_note() { return pressed_keys_.most_recent_note().next_ptr; }
  uint8_t played_note() { return pressed_keys_.played_note(0U).note; }
  uint8_t note_by_priority() { return pressed_keys_.played_note(0U).note; }

private:
  NoteStack<8> pressed_keys_;
};

} // namespace jamstack

#endif /* GROK_VOICE_MANAGER */
