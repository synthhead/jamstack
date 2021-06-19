#ifndef synth_dc_h_
#define synth_dc_h_

#include "SoundStream.h"
#include "jamstackcommons.h"
#include "utils/dspinst.h"

#if defined(__ARM_ARCH_7EM__)
static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c)
    __attribute__((always_inline, unused));
static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c)
{
  int r;
  r = substract_32_saturate(a, b);
  if(!get_q_psr())
    return (r / c);
  clr_q_psr();
  if(c == 0)
    r = 0;
  if(__builtin_abs(c) <= 1)
    return r;
  return (a / c) - (b / c);
}
#else

static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c)
    __attribute__((always_inline, unused));
static inline int32_t substract_int32_then_divide_int32(int32_t a, int32_t b, int32_t c)
{
  uint32_t diff;
  uint8_t  negative;

  if(a >= 0)
  {
    if(b >= 0)
    {
      return (a - b) / c;
    }
    else
    {
      diff     = a + (b * -1);
      negative = 0;
    }
  }
  else
  {
    if(b >= 0)
    {
      diff     = (a * -1) + b;
      negative = 1;
    }
    else
    {
      return (a - b) / c;
    }
  }
  if(c >= 0)
  {
    diff = diff / (uint32_t)c;
  }
  else
  {
    diff = diff / (uint32_t)(c * -1);
    negative ^= 1;
  }
  if(negative)
  {
    if(diff > 0x7FFFFFFF)
      return 0x80000000;
    return (int32_t)diff * -1;
  }
  else
  {
    if(diff > 0x7FFFFFFF)
      return 0x7FFFFFFF;
    return (int32_t)diff;
  }
}

#endif

class SoundSynthWaveformDc : public SoundStream
{
public:
  SoundSynthWaveformDc() : SoundStream(0, NULL), state(0), magnitude(0) {}

  void amplitude(float n)
  {
    if(n > 1.0)
      n = 1.0;
    else if(n < -1.0)
      n = -1.0;
    int32_t m = (int32_t)(n * 2147418112.0);
    IrqAllDisable();
    magnitude = m;
    state     = 0;
    IrqAllEnable();
  }

  void amplitude(float n, float milliseconds)
  {
    if(milliseconds <= 0.0)
    {
      amplitude(n);
      return;
    }
    if(n > 1.0)
      n = 1.0;
    else if(n < -1.0)
      n = -1.0;
    int32_t c = (int32_t)(milliseconds * (SOUND_SAMPLING_RATE_EXACT / 1000.0));
    if(c == 0)
    {
      amplitude(n);
      return;
    }
    int32_t t = (int32_t)(n * 2147418112.0);
    IrqAllDisable();
    target = t;
    if(target == magnitude)
    {
      state = 0;
      IrqAllEnable();
      return;
    }
    increment = substract_int32_then_divide_int32(target, magnitude, c);
    if(increment == 0)
    {
      increment = (target > magnitude) ? 1 : -1;
    }
    state = 1;
    IrqAllEnable();
  }
  float read(void)
  {
    int32_t m = magnitude;
    return (float)m * (1.0 / 2147418112.0);
  }
  virtual void update(void) override;

private:
  uint8_t state;
  int32_t magnitude;
  int32_t target;
  int32_t increment;
};

#endif
