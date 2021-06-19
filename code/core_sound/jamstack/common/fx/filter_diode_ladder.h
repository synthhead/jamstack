#pragma once
#ifndef JAMSTACK_FILTER_DIODE_LADDER
#define JAMSTACK_FILTER_DIODE_LADDER

#include <SoundStream.h>
#include <arm_math.h>
#include <algorithm>

namespace Jamstack
{
class FilterDiodeLadder : public SoundStream
{
public:
  FilterDiodeLadder(void) : SoundStream(1, inputQueueArray)
  {
    std::fill(z, z + 5, 0);
    setResonance(0.f);
    const float diodeResHpfCutoff_Hz = 40.f;
    setFeedbackHpfCutoff(diodeResHpfCutoff_Hz / SOUND_SAMPLING_RATE_EXACT * 2);
  }

  void setCutoff(const float freq) { cutoff = freq; }

  void setResonance(const float q)
  {
    k = 20.f * q;
    A = 1.f + 0.5f * k;
  }

  void setFeedbackHpfCutoff(const float freq)
  {
    const float K = freq * M_PI;
    ah            = (K - 2) / (K + 2);
    bh            = 2 / (K + 2);
  }

  void reset()
  {
    if(k < 17)
      std::fill(z, z + 5, 0);
  }

  void calcFilterParams() {}

  int16_t to_int16(float f)
  {
    if(f > 1.0)
      f = 1.0;
    if(f < -1.0)
      f = -1.0;
    return (int16_t)(f * 0x7fff);
  }

  virtual void update(void) override
  {
    sound_block_t *block;

    block = receiveWritable(0);

    if(!block)
      return;

    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      float curr_sample = block->data[i] / 32767.0f;

      const float a = M_PI * cutoff;

      const float ainv = 1 / a;
      const float a2   = a * a;
      const float b    = 2 * a + 1;
      const float b2   = b * b;
      const float c    = 1 / (2 * a2 * a2 - 4 * a2 * b2 + b2 * b2);
      const float g0   = 2 * a2 * a2 * c;
      const float g    = g0 * bh;

      const float s0 = (a2 * a * z[0] + a2 * b * z[1] + z[2] * (b2 - 2 * a2) * a + z[3] * (b2 - 3 * a2) * b) * c;
      const float s  = bh * s0 - z[4];

      float y5 = (g * curr_sample + s) / (1 + g * k);

      const float pre_clip = curr_sample - k * y5;
      const float y0       = pre_clip / (1 + abs(pre_clip));

      y5 = g * y0 + s;

      const float y4 = g0 * y0 + s0;
      const float y3 = (b * y4 - z[3]) * ainv;
      const float y2 = (b * y3 - a * y4 - z[2]) * ainv;
      const float y1 = (b * y2 - a * y3 - z[1]) * ainv;

      z[0] += 4 * a * (y0 - y1 + y2);
      z[1] += 2 * a * (y1 - 2 * y2 + y3);
      z[2] += 2 * a * (y2 - 2 * y3 + y4);
      z[3] += 2 * a * (y3 - 2 * y4);
      z[4] = bh * y4 + ah * y5;

      block->data[i] = to_int16(A * y4);
    }

    transmit(block);
    release(block);
  }

private:
  sound_block_t *inputQueueArray[1];

  float cutoff;
  float resonance;

  float k, A;
  float z[5];
  float ah, bh;
};
} // namespace Jamstack

#endif
