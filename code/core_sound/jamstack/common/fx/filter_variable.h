#ifndef filter_variable_h_
#define filter_variable_h_

#include "SoundStream.h"
#include "jamstackcommons.h"
#include "arm_math.h"

class SoundFilterStateVariable : public SoundStream
{
public:
  SoundFilterStateVariable() : SoundStream(2, inputQueueArray)
  {
    frequency(1000);
    octaveControl(1.0);
    resonance(0.707);
    state_inputprev = 0;
    state_lowpass   = 0;
    state_bandpass  = 0;
  }
  void frequency(float freq)
  {
    if(freq < 20.0)
      freq = 20.0;
    else if(freq > SOUND_SAMPLING_RATE_EXACT / 2.5)
      freq = SOUND_SAMPLING_RATE_EXACT / 2.5;
    setting_fcenter = (freq * (3.141592654 / (SOUND_SAMPLING_RATE_EXACT * 2.0))) * 2147483647.0;

    setting_fmult = sinf(freq * (3.141592654 / (SOUND_SAMPLING_RATE_EXACT * 2.0))) * 2147483647.0;
  }
  void resonance(float q)
  {
    if(q < 0.7)
      q = 0.7;
    else if(q > 5.0)
      q = 5.0;

    setting_damp = (1.0 / q) * 1073741824.0;
  }
  void octaveControl(float n)
  {
    if(n < 0.0)
      n = 0.0;
    else if(n > 6.9999)
      n = 6.9999;
    setting_octavemult = n * 4096.0;
  }
  virtual void update(void) override;

private:
  void           update_fixed(const int16_t *in, int16_t *lp, int16_t *bp, int16_t *hp);
  void           update_variable(const int16_t *in, const int16_t *ctl, int16_t *lp, int16_t *bp, int16_t *hp);
  int32_t        setting_fcenter;
  int32_t        setting_fmult;
  int32_t        setting_octavemult;
  int32_t        setting_damp;
  int32_t        state_inputprev;
  int32_t        state_lowpass;
  int32_t        state_bandpass;
  sound_block_t *inputQueueArray[2];
};

#endif
