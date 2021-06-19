#ifndef effect_line_h_
#define effect_line_h_

#include "SoundStream.h"
#include "jamstackcommons.h"
#include "utils/dspinst.h"

#define SAMPLES_PER_MSEC (SOUND_SAMPLING_RATE_EXACT / 1000.0)

class SoundFxLine : public SoundStream
{
public:
  SoundFxLine() : SoundStream(1, inputQueueArray)
  {
    state = 0;
    delay(0.0f);
    attack(10.5f);
    hold(2.5f);
    decay(35.0f);
    sustain(0.5f);
    release(300.0f);
    releaseNoteOn(5.0f);
  }
  void noteOn();
  void noteOff();
  void delay(float milliseconds) { delay_count = milliseconds2count(milliseconds); }
  void attack(float milliseconds)
  {
    attack_count = milliseconds2count(milliseconds);
    if(attack_count == 0)
      attack_count = 1;
  }
  void hold(float milliseconds) { hold_count = milliseconds2count(milliseconds); }
  void decay(float milliseconds)
  {
    decay_count = milliseconds2count(milliseconds);
    if(decay_count == 0)
      decay_count = 1;
  }
  void sustain(float level)
  {
    if(level < 0.0)
      level = 0;
    else if(level > 1.0)
      level = 1.0;
    sustain_mult = level * 1073741824.0;
  }
  void release(float milliseconds)
  {
    release_count = milliseconds2count(milliseconds);
    if(release_count == 0)
      release_count = 1;
  }
  void releaseNoteOn(float milliseconds)
  {
    release_forced_count = milliseconds2count(milliseconds);
    if(release_count == 0)
      release_count = 1;
  }
  bool isActive();
  bool isSustain();
  using SoundStream::release;
  virtual void update(void) override;

private:
  uint16_t milliseconds2count(float milliseconds)
  {
    if(milliseconds < 0.0)
      milliseconds = 0.0;
    uint32_t c = ((uint32_t)(milliseconds * SAMPLES_PER_MSEC) + 7) >> 3;
    if(c > 65535)
      c = 65535;
    return c;
  }
  sound_block_t *inputQueueArray[1];

  uint8_t  state;
  uint16_t count;
  int32_t  mult_hires;
  int32_t  inc_hires;

  uint16_t delay_count;
  uint16_t attack_count;
  uint16_t hold_count;
  uint16_t decay_count;
  int32_t  sustain_mult;
  uint16_t release_count;
  uint16_t release_forced_count;
};

#undef SAMPLES_PER_MSEC
#endif
