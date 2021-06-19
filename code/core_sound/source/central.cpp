#include "Shez.h"
#include "central.h"
#include "testseq.h"

namespace core_sound
{
//
void Central::Init()
{
  voiceManager.Init();
  testseq.Init();

  output = new SoundOutputI2S();
  // when I add SoundInputI2S, sometimes I can't easily reflash the chip:
  // "Chip initialization failed - Ef(34): Timed-out initializing flash."
  // Is there a data leakage? Or low memory, e.g. heap?
  input = new SoundInputI2S();

  player    = new AudioPlayMemory();
  outLeft   = new SoundAmplifier();
  outRight  = new SoundAmplifier();
  mainMixer = new SoundMixer4();

  outLeft->gain(0.8);
  outRight->gain(0.8);

  // for(int i = 0; i < 4; i++)
  //   mainMixer->gain(i, 1.f);

  // // multiKeturi = new MultiKeturi();
  // multiBandlim = new MultiBandlim();

  // // cords[6] = new SoundConnection(*multiKeturi->getOutput(), 0, *mainMixer, 2);
  // cords[6] = new SoundConnection(*multiBandlim->getOutput(), 0, *mainMixer, 2);
  // cords[5] = new SoundConnection(*player, 0, *mainMixer, 1);
  // cords[4] = new SoundConnection(*input, 0, *mainMixer, 0);

  // cords[3] = new SoundConnection(*mainMixer, *outLeft);
  // cords[2] = new SoundConnection(*mainMixer, *outRight);

  synth        = new Synth();
  audioClocker = new jamstack::Clocker();

  // Clock
  cords[4] = new SoundConnection(*synth->output_0(), *audioClocker);
  //
  cords[3] = new SoundConnection(*synth->output_0(), *outLeft);
  cords[2] = new SoundConnection(*synth->output_1(), *outRight);
  cords[1] = new SoundConnection(*outLeft, 0, *output, 1);
  cords[0] = new SoundConnection(*outRight, 0, *output, 0);
}

void Central::InternalNoteOn(uint8_t ch, uint8_t note, uint8_t velocity)
{
  usbMIDI.sendNoteOn(note, velocity, 0);
  NoteOn(ch, note + note_offset_, velocity);
}

void Central::InternalNoteOff(uint8_t ch, uint8_t note, uint8_t velocity)
{
  usbMIDI.sendNoteOff(note, velocity, 0);
  NoteOff(ch, note + note_offset_, velocity);
}

void Central::NoteOn(uint8_t ch, uint8_t note, uint8_t velocity)
{
  // PRINTF("Note On: %d %d %d\r\n", channel, note, velocity);
  // float freq = powf(2.0, note * (1.0 / 12.0) + 3.0313597);
  // multiKeturi->noteOn(note);
  // player->play(SoundSampleHihat);
  // multiBandlim->noteOn(freq);

  // IrqAudioDisable();
  synth->onNoteOn(ch, note, velocity);
  voiceManager.NotaOn(ch, note, velocity);
  // IrqAudioEnable();

  PRINTF("NoteOn stack size: %i, recent note: %i\r\n", voiceManager.size(), voiceManager.most_recent_note());
}

void Central::NoteOff(uint8_t ch, uint8_t note, uint8_t velocity)
{
  // PRINTF("Note Off: %d %d %d\n", channel, note, velocity);
  // multiKeturi->noteOff();
  // multiBandlim->noteOff();
  // synth->onNoteOff(channel, note, velocity);

  // IrqAudioDisable();
  uint8_t before_note = voiceManager.most_recent_note();
  voiceManager.NotaOff(ch, note, velocity);
  uint8_t after_note = voiceManager.most_recent_note();
  PRINTF("NoteOff stack size: %i, before note: %i, recent note: %i\r\n", voiceManager.size(), before_note, after_note);

  if(after_note == 0xFF)
  {
    synth->onNoteOff(ch, before_note, velocity);
  }
  else if(after_note != before_note)
  {
    synth->onNoteOff(ch, before_note, velocity);
    synth->onNoteOn(ch, after_note, 127U); // Todo: velocity
  }
}

void Central::ControlChange(uint8_t ch, uint8_t control, uint8_t data)
{
  // PRINTF("CC %d %d %d\n", channel, control, data);

  // Encoders
  if(control == 0)
  {
    usbMIDI.sendControlChange(10, data, 0);
  }
  else if(control == 1)
  {
    usbMIDI.sendControlChange(11, data, 0);
  }
  else if(control == 2)
  {
    usbMIDI.sendControlChange(12, data, 0);
  }
  else if(control == 3)
  {
    usbMIDI.sendControlChange(13, data, 0);

    // float val = data / 127.f;
    // outLeft->gain(val);
    // outRight->gain(val);
  }
  else if(control == 4)
  {
    usbMIDI.sendControlChange(14, data, 0);
  }
  else if(control == 5)
  {
    usbMIDI.sendControlChange(15, data, 0);
  }
  else if(control == 6)
  {
    usbMIDI.sendControlChange(16, data, 0);
  }
  else if(control == 7)
  {
    usbMIDI.sendControlChange(17, data, 0);
    // testseq.Tempo(data);
  }
  // Buttons
  else if(control >= 10 && control <= 25)
  {
    FunctionKey(control - 10, data);
  }
  // Enc Buttons
  else if(control > 29 && control < 38)
  {
    usbMIDI.sendControlChange(50 + control - 30, data, 0);
  }
  // Left bottom L
  else if(control == 26)
  {
    if(data == 1)
    {
      usbMIDI.sendControlChange(0, 127, 0);
      AdjustNoteOffset(-12);
    }
    else
    {
      usbMIDI.sendControlChange(0, 0, 0);
    }
  }
  // Left Bottom R
  else if(control == 28)
  {
    if(data == 1)
    {
      usbMIDI.sendControlChange(1, 127, 0);
      AdjustNoteOffset(12);
    }
    else
    {
      usbMIDI.sendControlChange(1, 0, 0);
    }
  }
  else if(control == 40)
  {
    usbMIDI.sendControlChange(20, data / 2, 0);
  }
  else if(control == 41)
  {
    usbMIDI.sendControlChange(21, data, 0);
  }
  else
  {
    synth->onControlChange(ch, control, data);
  }
}

volatile uint8_t last_func_state = 0;

void Central::FunctionKey(uint8_t key, uint8_t state)
{
  // static StateFunc last_state_func = core_sound::StateFunc::synth_a;
  PRINTF("FUNC: state: %u, last func %u \r\n", state, last_func_state);

  if(state == 0 && last_func_state == 1)
  {
    usbMIDI.sendProgramChange(key, 0);

    switch(key)
    {
      case 0:
        state_func = core_sound::StateFunc::synth_a;
        PRINTF("Synth A \r\n");
        break;
      case 1:
        state_func = core_sound::StateFunc::synth_b;
        PRINTF("Synth B \r\n");
        break;
      case 2:
        state_func = core_sound::StateFunc::synth_c;
        PRINTF("Synth C \r\n");
        break;
      case 3:
        state_func = core_sound::StateFunc::synth_d;
        PRINTF("Synth D \r\n");
        break;

      case 4:
        state_func = core_sound::StateFunc::drum_a;
        PRINTF("Drum A \r\n");
        break;
      case 5:
        state_func = core_sound::StateFunc::drum_b;
        PRINTF("Drum B \r\n");
        break;
      case 6:
        state_func = core_sound::StateFunc::drum_c;
        PRINTF("Drum C \r\n");
        break;
      case 7:
        state_func = core_sound::StateFunc::drum_d;
        PRINTF("Drum D \r\n");
        break;

      case 8:
        state_func = core_sound::StateFunc::fx_a;
        PRINTF("FX A \r\n");
        break;
      case 9:
        state_func = core_sound::StateFunc::fx_b;
        PRINTF("FX B \r\n");
        break;
      case 10:
        state_func = core_sound::StateFunc::fx_c;
        PRINTF("FX C \r\n");
        break;
      case 11:
        state_func = core_sound::StateFunc::fx_d;
        PRINTF("FX D \r\n");
        break;

      case 12:
        state_func = core_sound::StateFunc::mode_a;
        PRINTF("Mode A \r\n");
        break;
      case 13:
        state_func = core_sound::StateFunc::mode_b;
        PRINTF("Mode B \r\n");
        break;
      case 14:
        state_func = core_sound::StateFunc::mode_c;
        PRINTF("Mode C \r\n");
        break;
      case 15:
        state_func = core_sound::StateFunc::mode_d;
        PRINTF("Mode D \r\n");
        break;

      default: break;
    }
  }

  last_func_state = state;

  // if((key == 15) && (state == 1))
  // {
  //   testseq.ToggleRunning();
  // }
  // else if((key == 8) && (state == 1))
  // {
  //   PRINTF("Recording \r\n");
  // }
}

void Central::AdjustNoteOffset(int8_t offset)
{
  auto mrn = voiceManager.most_recent_note();
  if(mrn != 255)
  {
    NoteOff(0, mrn, 127U);
    NoteOn(0, mrn + offset, 127U);
  }

  note_offset_ += offset;
}

Central central;

} // namespace core_sound
