#ifndef synth_waveform_h_
#define synth_waveform_h_

#include "SoundStream.h"
#include "jamstackcommons.h"
#include "arm_math.h"
#include "../data/data_waveforms.h"

#define WAVEFORM_SINE              0
#define WAVEFORM_SAWTOOTH          1
#define WAVEFORM_SQUARE            2
#define WAVEFORM_TRIANGLE          3
#define WAVEFORM_ARBITRARY         4
#define WAVEFORM_PULSE             5
#define WAVEFORM_SAWTOOTH_REVERSE  6
#define WAVEFORM_SAMPLE_HOLD       7
#define WAVEFORM_TRIANGLE_VARIABLE 8

class SoundSynthWaveform : public SoundStream
{
public:
  SoundSynthWaveform(void)
  : SoundStream(0, NULL),
    phase_accumulator(0),
    phase_increment(0),
    phase_offset(0),
    magnitude(0),
    pulse_width(0x40000000),
    arbdata(NULL),
    sample(0),
    tone_type(WAVEFORM_SINE),
    tone_offset(0)
  {
  }

  void frequency(float freq)
  {
    if(freq < 0.0)
    {
      freq = 0.0;
    }
    else if(freq > SOUND_SAMPLING_RATE_EXACT / 2)
    {
      freq = SOUND_SAMPLING_RATE_EXACT / 2;
    }
    phase_increment = freq * (4294967296.0 / SOUND_SAMPLING_RATE_EXACT);
    if(phase_increment > 0x7FFE0000u)
      phase_increment = 0x7FFE0000;
  }
  void phase(float angle)
  {
    if(angle < 0.0)
    {
      angle = 0.0;
    }
    else if(angle > 360.0)
    {
      angle = angle - 360.0;
      if(angle >= 360.0)
        return;
    }
    phase_offset = angle * (4294967296.0 / 360.0);
  }
  void amplitude(float n)
  {
    if(n < 0)
    {
      n = 0;
    }
    else if(n > 1.0)
    {
      n = 1.0;
    }
    magnitude = n * 65536.0;
  }
  void offset(float n)
  {
    if(n < -1.0)
    {
      n = -1.0;
    }
    else if(n > 1.0)
    {
      n = 1.0;
    }
    tone_offset = n * 32767.0;
  }
  void pulseWidth(float n)
  {
    if(n < 0)
    {
      n = 0;
    }
    else if(n > 1.0)
    {
      n = 1.0;
    }
    pulse_width = n * 4294967296.0;
  }
  void begin(short t_type)
  {
    phase_offset = 0;
    tone_type    = t_type;
  }
  void begin(float t_amp, float t_freq, short t_type)
  {
    amplitude(t_amp);
    frequency(t_freq);
    phase_offset = 0;
    tone_type    = t_type;
  }
  void         arbitraryWaveform(const int16_t *data, float maxFreq) { arbdata = data; }
  virtual void update(void) override;

private:
  uint32_t       phase_accumulator;
  uint32_t       phase_increment;
  uint32_t       phase_offset;
  int32_t        magnitude;
  uint32_t       pulse_width;
  const int16_t *arbdata;
  int16_t        sample;
  short          tone_type;
  int16_t        tone_offset;
};

class SoundSynthWaveformModulated : public SoundStream
{
public:
  SoundSynthWaveformModulated(void)
  : SoundStream(2, inputQueueArray),
    phase_accumulator(0),
    phase_increment(0),
    modulation_factor(32768),
    magnitude(0),
    arbdata(NULL),
    sample(0),
    tone_offset(0),
    tone_type(WAVEFORM_SINE),
    modulation_type(0)
  {
  }

  void frequency(float freq)
  {
    if(freq < 0.0)
    {
      freq = 0.0;
    }
    else if(freq > SOUND_SAMPLING_RATE_EXACT / 2)
    {
      freq = SOUND_SAMPLING_RATE_EXACT / 2;
    }
    phase_increment = freq * (4294967296.0 / SOUND_SAMPLING_RATE_EXACT);
    if(phase_increment > 0x7FFE0000u)
      phase_increment = 0x7FFE0000;
  }
  void amplitude(float n)
  {
    if(n < 0)
    {
      n = 0;
    }
    else if(n > 1.0)
    {
      n = 1.0;
    }
    magnitude = n * 65536.0;
  }
  void offset(float n)
  {
    if(n < -1.0)
    {
      n = -1.0;
    }
    else if(n > 1.0)
    {
      n = 1.0;
    }
    tone_offset = n * 32767.0;
  }
  void begin(short t_type) { tone_type = t_type; }
  void begin(float t_amp, float t_freq, short t_type)
  {
    amplitude(t_amp);
    frequency(t_freq);
    tone_type = t_type;
  }
  void arbitraryWaveform(const int16_t *data, float maxFreq) { arbdata = data; }
  void frequencyModulation(float octaves)
  {
    if(octaves > 12.0)
    {
      octaves = 12.0;
    }
    else if(octaves < 0.1)
    {
      octaves = 0.1;
    }
    modulation_factor = octaves * 4096.0;
    modulation_type   = 0;
  }
  void phaseModulation(float degrees)
  {
    if(degrees > 9000.0)
    {
      degrees = 9000.0;
    }
    else if(degrees < 30.0)
    {
      degrees = 30.0;
    }
    modulation_factor = degrees * (65536.0 / 180.0);
    modulation_type   = 1;
  }
  virtual void update(void);

private:
  sound_block_t *inputQueueArray[2];
  uint32_t       phase_accumulator;
  uint32_t       phase_increment;
  uint32_t       modulation_factor;
  int32_t        magnitude;
  const int16_t *arbdata;
  uint32_t       phasedata[SOUND_BLOCK_SAMPLES];
  int16_t        sample;
  int16_t        tone_offset;
  uint8_t        tone_type;
  uint8_t        modulation_type;
};

#endif
