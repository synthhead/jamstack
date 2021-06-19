#ifndef synth_bandlimitedosc_h
#define synth_bandlimitedosc_h

#include "SoundStream.h"
#include "jamstackcommons.h"
#include "utils/dspinst.h"
#include "arm_math.h"

class AudioBandlimitedOsc : public SoundStream
{
public:
  AudioBandlimitedOsc() : SoundStream(6, inputQueueArray) {}

  void amplitude(uint8_t oscillator, float a)
  {
    if(a < -1)
    {
      a = -1;
    }
    else if(a > 1.0)
    {
      a = 1.0;
    }
    switch(oscillator)
    {
      case 1:
      {
        osc1_gain = a;
      }
      break;

      case 2:
      {
        osc2_gain = a;
      }
      break;

      case 3:
      {
        osc3_gain = a;
      }
      break;
    }
  }

  void frequency(uint8_t oscillator, float freq)
  {
    if(freq < 0.0)
    {
      freq = 0.0;
    }
    else if(freq > SOUND_SAMPLING_RATE_EXACT / 2)
    {
      freq = SOUND_SAMPLING_RATE_EXACT / 2;
    }

    switch(oscillator)
    {
      case 1:
      {
        if(osc1_portamentoSamples > 0 && notesPlaying > 0)
        {
          osc1_portamentoIncrement     = (freq - frequency1) / osc1_portamentoSamples;
          osc1_currentPortamentoSample = 0;
        }
        else
        {
          frequency1 = freq;
        }

        osc1_dt = frequency1 / SOUND_SAMPLING_RATE_EXACT;
      }
      break;

      case 2:
      {
        if(osc2_portamentoSamples > 0 && notesPlaying > 0)
        {
          osc2_portamentoIncrement     = (freq - frequency2) / (float)osc2_portamentoSamples;
          osc2_currentPortamentoSample = 0;
        }
        else
        {
          frequency2 = freq;
        }
        osc2_dt = frequency2 / SOUND_SAMPLING_RATE_EXACT;
      }
      break;

      case 3:
      {
        if(osc3_portamentoSamples > 0 && notesPlaying > 0)
        {
          osc3_portamentoIncrement     = (freq - frequency3) / (float)osc3_portamentoSamples;
          osc3_currentPortamentoSample = 0;
        }
        else
        {
          frequency3 = freq;
        }
        osc3_dt = frequency3 / SOUND_SAMPLING_RATE_EXACT;
      }
      break;
    }
  }

  void phase(uint8_t oscillator, float t)
  {
    t -= floorf(t);

    switch(oscillator)
    {
      case 1:
      {
        osc1_t = t;
      }
      break;

      case 2:
      {
        osc2_t = t;
      }
      break;

      case 3:
      {
        osc3_t = t;
      }
      break;
    }
  }

  void waveform(uint8_t oscillator, uint8_t wform)
  {
    switch(oscillator)
    {
      case 1:
      {
        osc1_waveform = wform;
      }
      break;

      case 2:
      {
        osc2_waveform = wform;
      }
      break;

      case 3:
      {
        osc3_waveform = wform;
      }
      break;
    }
  }

  void pulseWidth(uint8_t oscillator, float pulseWidth)
  {
    if(pulseWidth < 0.01f)
    {
      pulseWidth = 0.01f;
    }
    else if(pulseWidth > 0.99f)
    {
      pulseWidth = 0.99f;
    }

    switch(oscillator)
    {
      case 1:
      {
        pulseWidth1     = pulseWidth;
        osc1_pulseWidth = pulseWidth;
      }
      break;

      case 2:
      {
        pulseWidth2     = pulseWidth;
        osc2_pulseWidth = pulseWidth;
      }
      break;

      case 3:
      {
        pulseWidth3     = pulseWidth;
        osc3_pulseWidth = pulseWidth;
      }
      break;
    }
  }

  void portamentoTime(uint8_t oscillator, float seconds)
  {
    switch(oscillator)
    {
      case 1:
      {
        osc1_portamentoTime    = seconds;
        osc1_portamentoSamples = floorf(seconds * SOUND_SAMPLING_RATE_EXACT);
      }
      break;

      case 2:
      {
        osc2_portamentoTime    = seconds;
        osc2_portamentoSamples = floorf(seconds * SOUND_SAMPLING_RATE_EXACT);
      }
      break;

      case 3:
      {
        osc3_portamentoTime    = seconds;
        osc3_portamentoSamples = floorf(seconds * SOUND_SAMPLING_RATE_EXACT);
      }
      break;
    }
  }

  void fmAmount(uint8_t oscillator, float octaves)
  {
    if(octaves > 12.0)
    {
      octaves = 12.0;
    }
    else if(octaves < 0.1)
    {
      octaves = 0.1;
    }
    switch(oscillator)
    {
      case 1:
      {
        osc1_pitchModAmount = octaves * 4096.0;
        ;
      }
      break;

      case 2:
      {
        osc2_pitchModAmount = octaves * 4096.0;
        ;
      }
      break;

      case 3:
      {
        osc3_pitchModAmount = octaves * 4096.0;
        ;
      }
      break;
    }
  }

  void pwmAmount(uint8_t oscillator, float amount)
  {
    switch(oscillator)
    {
      case 1:
      {
        osc1_pwmAmount = amount;
      }
      break;

      case 2:
      {
        osc2_pwmAmount = amount;
      }
      break;

      case 3:
      {
        osc3_pwmAmount = amount;
      }
      break;
    }
  }

  void addNote() { notesPlaying++; }

  void removeNote()
  {
    if(notesPlaying > 0)
    {
      notesPlaying--;
    }
  }

  virtual void update(void) override;

  inline void osc1Step();
  inline void osc2Step();
  inline void osc2Sync(float x);
  inline void osc3Step();
  inline void osc3Sync(float x);

  sound_block_t *inputQueueArray[6];

private:
  uint8_t notesPlaying;

  float    frequency1               = 0.0f;
  float    pulseWidth1              = 0.0f;
  float    osc1_freq                = 0.0f;
  float    osc1_pulseWidth          = 0.5f;
  float    osc1_gain                = 0.0f;
  uint8_t  osc1_waveform            = 1;
  float    osc1_output              = 0.0f;
  float    osc1_blepDelay           = 0.0f;
  float    osc1_t                   = 0.0f;
  float    osc1_dt                  = 0.0f;
  float    osc1_widthDelay          = 0.0f;
  bool     osc1_pulseStage          = false;
  float    osc1_pwmAmount           = 0.5f;
  uint32_t osc1_pitchModAmount      = 4096;
  float    osc1_portamentoTime      = 0.0f;
  float    osc1_portamentoIncrement = 0.0f;
  uint64_t osc1_portamentoSamples   = 0;
  uint64_t osc1_currentPortamentoSample;

  float    frequency2                   = 0.0f;
  float    pulseWidth2                  = 0.0f;
  float    osc2_freq                    = 0.0f;
  float    osc2_pulseWidth              = 0.5f;
  float    osc2_gain                    = 0.0f;
  uint8_t  osc2_waveform                = 1;
  float    osc2_output                  = 0.0f;
  float    osc2_blepDelay               = 0.0f;
  float    osc2_t                       = 0.0f;
  float    osc2_dt                      = 0.0f;
  float    osc2_widthDelay              = 0.0f;
  bool     osc2_pulseStage              = false;
  float    osc2_pwmAmount               = 0.5f;
  uint32_t osc2_pitchModAmount          = 4096;
  float    osc2_portamentoTime          = 0.0f;
  float    osc2_portamentoIncrement     = 0.0f;
  uint64_t osc2_portamentoSamples       = 0;
  uint64_t osc2_currentPortamentoSample = 0;

  float    frequency3                   = 0.0f;
  float    pulseWidth3                  = 0.0f;
  float    osc3_freq                    = 0.0f;
  float    osc3_pulseWidth              = 0.5f;
  float    osc3_gain                    = 0.0f;
  uint8_t  osc3_waveform                = 1;
  float    osc3_output                  = 0.0f;
  float    osc3_blepDelay               = 0.0f;
  float    osc3_t                       = 0.0f;
  float    osc3_dt                      = 0.0f;
  float    osc3_widthDelay              = 0.0f;
  bool     osc3_pulseStage              = false;
  float    osc3_pwmAmount               = 0.5f;
  uint32_t osc3_pitchModAmount          = 4096;
  float    osc3_portamentoTime          = 0.0f;
  float    osc3_portamentoIncrement     = 0.0f;
  uint64_t osc3_portamentoSamples       = 0;
  uint64_t osc3_currentPortamentoSample = 0;
};

#endif
