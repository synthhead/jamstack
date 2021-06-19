#ifndef synth_blosc_h
#define synth_blosc_h

#include "SoundStream.h"
#include "jamstackcommons.h"

#include <stdint.h>

class SoundSynthBlosc : public SoundStream
{
public:
  SoundSynthBlosc() : SoundStream(0, NULL)
  {
    sampling_freq_  = SOUND_SAMPLING_RATE_EXACT;
    half_sr_        = 0.5 * sampling_freq_;
    quarter_sr_     = sampling_freq_ * 0.25;
    sec_per_sample_ = 1.0 / sampling_freq_;
    two_over_sr_    = (float)(2.0 / sampling_freq_);
    four_over_sr_   = 4.0 / sampling_freq_;

    freq_ = 440;
    amp_  = 0.5;
    pw_   = 0.5;
    iota_ = 0;
    mode_ = WAVE_TRIANGLE;

    for(int i = 0; i < 2; i++)
    {
      rec0_[i] = rec1_[i] = vec0_[i] = vec1_[i] = 0.0;
    }

    for(int i = 0; i < 4096; i++)
    {
      vec2_[i] = 0.0;
    }
  }
  ~SoundSynthBlosc();

  enum Waveforms
  {
    WAVE_TRIANGLE,
    WAVE_SAW,
    WAVE_SQUARE,
    WAVE_OFF,
  };

  inline void setFreq(float freq) { freq_ = freq; }
  inline void setAmp(float amp) { amp_ = amp; }
  inline void setPw(float pw) { pw_ = pw; }
  inline void setWaveform(uint8_t wave) { mode_ = wave; }
  void        Reset();

  virtual void update(void) override;

private:
  sound_block_t *inputQueueArray[1];

  float   rec0_[2], rec1_[2], vec0_[2], vec1_[2], vec2_[4096];
  float   freq_, amp_, pw_, sampling_freq_, half_sr_, quarter_sr_;
  float   sec_per_sample_, two_over_sr_, four_over_sr_;
  uint8_t mode_;
  int     iota_;

  float imguiResult;

  float ProcessSquare();
  float ProcessTriangle();
  float ProcessSaw();
};

#endif
