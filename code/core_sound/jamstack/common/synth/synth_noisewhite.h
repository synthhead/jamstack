#ifndef synth_whitenoise_h_
#define synth_whitenoise_h_
#include "SoundStream.h"
#include "utils/dspinst.h"

class SoundSynthNoiseWhite : public SoundStream
{
public:
  SoundSynthNoiseWhite() : SoundStream(0, NULL)
  {
    level = 0;
    seed  = 1 + instance_count++;
  }
  void amplitude(float n)
  {
    if(n < 0.0)
      n = 0.0;
    else if(n > 1.0)
      n = 1.0;
    level = (int32_t)(n * 65536.0);
  }

  virtual void update(void) override;

private:
  int32_t         level;
  uint32_t        seed;
  static uint16_t instance_count;
};

#endif
