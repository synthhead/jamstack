#pragma once
#ifndef JAMSTACK_CENTRAL
#define JAMSTACK_CENTRAL

#include <stdint.h>
#include "fsl_debug_console.h"
#include "Jamstack.h"
#include "Synth.h"

namespace core_sound
{
//
const jamstack::NotePriority kNotePriority = jamstack::NotePriority::LAST;
static_assert(kNotePriority == jamstack::NotePriority::LAST, "kNotePriority must be LAST");

enum class StateFunc
{
  synth_a,
  synth_b,
  synth_c,
  synth_d,
  drum_a,
  drum_b,
  drum_c,
  drum_d,
  fx_a,
  fx_b,
  fx_c,
  fx_d,
  mode_a,
  mode_b,
  mode_c,
  mode_d,
};

class Central
{
public:
  Central() {}
  ~Central() {}

  /**
     * Initialize Central
     */
  void Init();
  void InternalNoteOn(uint8_t ch, uint8_t note, uint8_t velocity);
  void InternalNoteOff(uint8_t ch, uint8_t note, uint8_t velocity);
  void NoteOn(uint8_t ch, uint8_t note, uint8_t velocity);
  void NoteOff(uint8_t ch, uint8_t note, uint8_t velocity);
  void ControlChange(uint8_t ch, uint8_t control, uint8_t data);
  void FunctionKey(uint8_t key, uint8_t state);

  /**
     * Adjusts pitch offset for incoming note
     *
     * @param offset in
     * @return Return description
     */

  void AdjustNoteOffset(int8_t offset);

private:
  int8_t             note_offset_ = 36;
  SoundOutputI2S *   output       = nullptr;
  SoundInputI2S *    input        = nullptr;
  Synth *            synth        = nullptr;
  jamstack::Clocker *audioClocker = nullptr;
  // MultiBandlim*    multiBandlim = nullptr;
  // MultiKeturi * multiKeturi = nullptr;
  AudioPlayMemory *player = nullptr;

  SoundMixer4 *    mainMixer = nullptr;
  SoundAmplifier * outLeft   = nullptr;
  SoundAmplifier * outRight  = nullptr;
  SoundConnection *cords[8];

  jamstack::VoiceManager voiceManager;
  // jamstack::Tempo        tempo;
  StateFunc state_func = core_sound::StateFunc::synth_a;
};

extern Central central;

} // namespace core_sound

#endif // JAMSTACK_CENTRAL
