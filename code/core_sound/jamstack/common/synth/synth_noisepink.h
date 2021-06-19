

#ifndef synth_pinknoise_h_
#define synth_pinknoise_h_

#include "SoundStream.h"
#include "utils/dspinst.h"

class SoundSynthNoisePink : public SoundStream
{
public:
  SoundSynthNoisePink() : SoundStream(0, NULL)
  {
    plfsr = 0x5EED41F5 + instance_cnt++;
    paccu = 0;
    pncnt = 0;
    pinc  = 0x0CCC;
    pdec  = 0x0CCC;
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
  static const uint8_t pnmask[256];
  static const int32_t pfira[64];
  static const int32_t pfirb[64];
  static int16_t       instance_cnt;
  int32_t              plfsr;
  int32_t              pinc;
  int32_t              pdec;
  int32_t              paccu;
  uint8_t              pncnt;
  int32_t              level;
};

#endif
