#ifndef synth_main_entry_h
#define synth_main_entry_h

#include "SoundStream.h"
#include "Jamstack.h"
#include "utils/ArduinoJson-v6.16.1.h"
#include "com_patcher.h"

#include <memory>

class Synth
{
public:
  Synth();
  ~Synth();

  SoundAmplifier *input_0() { return static_cast<SoundAmplifier *>(input_0_.get()); }
  SoundMixer4 *   output_0() { return static_cast<SoundMixer4 *>(output_0_.get()); }
  SoundMixer4 *   output_1() { return static_cast<SoundMixer4 *>(output_1_.get()); }

  void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
  void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
  void onControlChange(uint8_t channel, uint8_t control, uint8_t data);
  void onProgramChange(uint8_t channel, uint8_t control, uint8_t data);
  void onPitchbendChange(uint8_t channel, uint8_t control, uint8_t data);
  void onSysex(const uint8_t *data, uint16_t length, bool last);

private:
  std::unique_ptr<SoundAmplifier> input_0_;
  std::unique_ptr<SoundMixer4>    output_0_;
  std::unique_ptr<SoundMixer4>    output_1_;

  std::unique_ptr<BasicSynth>        basicSynth_;
  std::unique_ptr<Jamstack::Patcher> patcher_1_;
  std::unique_ptr<SoundConnection>   cords_[8];

  StaticJsonDocument<2048> jsonBuffer;

  void processJson(void);
};

#endif
