#ifndef synth_sine_h_
#define synth_sine_h_

#include "SoundStream.h"

namespace Jamstack
{
class SoundSynthWaveformSine : public SoundStream
{
public:
  SoundSynthWaveformSine() : SoundStream(0, NULL), magnitude(16384), phase_increment(21426140) {}

  void frequency(float freq)
  {
    if(freq < 0.0)
      freq = 0.0;
    else if(freq > SOUND_SAMPLING_RATE_EXACT / 2)
      freq = SOUND_SAMPLING_RATE_EXACT / 2;
    phase_increment = freq * (4294967296.0 / SOUND_SAMPLING_RATE_EXACT);
  }
  void phase(float angle)
  {
    if(angle < 0.0)
      angle = 0.0;
    else if(angle > 360.0)
    {
      angle = angle - 360.0;
      if(angle >= 360.0)
        return;
    }
    phase_accumulator = angle * (4294967296.0 / 360.0);
  }
  void amplitude(float n)
  {
    if(n < 0)
      n = 0;
    else if(n > 1.0)
      n = 1.0;
    magnitude = n * 65536.0;
  }
  virtual void update(void) override;

  enum Params
  {
    FREQUENCY,
    PHASE,
    AMPLITUDE
  };

private:
  int32_t  magnitude;
  uint32_t phase_accumulator;
  uint32_t phase_increment;
};

class SoundSynthWaveformSineHires : public SoundStream
{
public:
  SoundSynthWaveformSineHires() : SoundStream(0, NULL), magnitude(16384) {}
  void frequency(float freq)
  {
    if(freq < 0.0)
      freq = 0.0;
    else if(freq > SOUND_SAMPLING_RATE_EXACT / 2)
      freq = SOUND_SAMPLING_RATE_EXACT / 2;
    phase_increment = freq * (4294967296.0 / SOUND_SAMPLING_RATE_EXACT);
  }
  void phase(float angle)
  {
    if(angle < 0.0)
      angle = 0.0;
    else if(angle > 360.0)
    {
      angle = angle - 360.0;
      if(angle >= 360.0)
        return;
    }
    phase_accumulator = angle * (4294967296.0 / 360.0);
  }
  void amplitude(float n)
  {
    if(n < 0)
      n = 0;
    else if(n > 1.0)
      n = 1.0;
    magnitude = n * 65536.0;
  }
  virtual void update(void) override;

  enum Params
  {
    FREQUENCY,
    PHASE,
    AMPLITUDE
  };

private:
  uint32_t phase_accumulator;
  uint32_t phase_increment;
  int32_t  magnitude;
};

class SoundSynthWaveformSineModulated : public SoundStream
{
public:
  SoundSynthWaveformSineModulated() : SoundStream(1, inputQueueArray), magnitude(16384) {}

  void frequency(float freq)
  {
    if(freq < 0.0)
      freq = 0.0;
    else if(freq > SOUND_SAMPLING_RATE_EXACT / 4)
      freq = SOUND_SAMPLING_RATE_EXACT / 4;
    phase_increment = freq * (4294967296.0 / SOUND_SAMPLING_RATE_EXACT);
  }
  void phase(float angle)
  {
    if(angle < 0.0)
      angle = 0.0;
    else if(angle > 360.0)
    {
      angle = angle - 360.0;
      if(angle >= 360.0)
        return;
    }
    phase_accumulator = angle * (4294967296.0 / 360.0);
  }
  void amplitude(float n)
  {
    if(n < 0)
      n = 0;
    else if(n > 1.0)
      n = 1.0;
    magnitude = n * 65536.0;
  }
  virtual void update(void) override;

  enum Params
  {
    FREQUENCY,
    PHASE,
    AMPLITUDE
  };

private:
  uint32_t       phase_accumulator;
  uint32_t       phase_increment;
  sound_block_t *inputQueueArray[1];
  int32_t        magnitude;
};
} // namespace Jamstack

#endif
