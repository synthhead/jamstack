#ifndef JAMSTACK_FILTER_ZDF_TRANSISTOR_LADDER
#define JAMSTACK_FILTER_ZDF_TRANSISTOR_LADDER

#include "SoundStream.h"
#include "jamstackcommons.h"
#include "arm_math.h"

class SoundFilterZdfTransistorLadder : public SoundStream
{
public:
  SoundFilterZdfTransistorLadder() : SoundStream(2, inputQueueArray) {}
  ~SoundFilterZdfTransistorLadder() {}

  void  Init() {}
  float set_r(const float val) { r_ = val; }

  inline float tanhXdX(float x)
  {
    float a = x * x;

    return ((a + 105) * a + 945) / ((15 * a + 420) * a + 945);
  }

  virtual void update()
  {
    sound_block_t *input_block = nullptr, *control_block = nullptr;

    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      float in1 = input_block->data[i] * (1.0f / 32768.0f);
      float in2 = control_block->data[i] * (1.0f / 32768.0f);

      float f = tan(PI * (in2 * 1. / SOUND_SAMPLING_RATE));

      float ih = 0.5 * (in1 + z1_);

      z1_ = in1;

      float t0 = tanhXdX(ih - r_ * s3_);
      float t1 = tanhXdX(s0_);
      float t2 = tanhXdX(s1_);
      float t3 = tanhXdX(s2_);
      float t4 = tanhXdX(s3_);

      float g0 = 1 / (1 + f * t1);
      float g1 = 1 / (1 + f * t2);
      float g2 = 1 / (1 + f * t3);
      float g3 = 1 / (1 + f * t4);

      float f3 = f * t3 * g3;
      float f2 = f * t2 * g2 * f3;
      float f1 = f * t1 * g1 * f2;
      float f0 = f * t0 * g0 * f1;

      float y3 = (g3 * s3_ + f3 * g2 * s2_ + f2 * g1 * s1_ + f1 * g0 * s0_ + f0 * in1) / (1 + r_ * f0);

      float xx = t0 * (in1 - r_ * y3);
      float y0 = t1 * g0 * (s0_ + f * xx);
      float y1 = t2 * g1 * (s1_ + f * y0);
      float y2 = t3 * g2 * (s2_ + f * y1);

      s0_ += 2 * f * (xx - y0);
      s1_ += 2 * f * (y0 - y1);
      s2_ += 2 * f * (y1 - y2);
      s3_ += 2 * f * (y2 - t4 * y3);

      input_block->data[i] = y3 * 32767.0f;
    }

    transmit(input_block);
    release(input_block);
  }

private:
  sound_block_t *inputQueueArray[2];

  float r_  = 0.f;
  float z1_ = 0.f, s0_ = 0.f, s1_ = 0.f, s2_ = 0.f, s3_ = 0.f;
};
#endif
