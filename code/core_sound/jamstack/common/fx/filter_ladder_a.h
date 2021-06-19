#pragma once

#ifndef filter_ladder_a_h_
#define filter_ladder_a_h_

#include <SoundStream.h>
#include <arm_math.h>

namespace Jamstack
{
class SoundFilterLadderA : public SoundStream
{
public:
  SoundFilterLadderA(void) : SoundStream(1, inputQueueArray), thermal(0.000025)
  {
    setCutoff(1000.0f);
    setResonance(0.1f);
  }

  void setResonance(float r)
  {
    resonance = r;
    resQuad   = 4.0 * resonance * acr;
  }

  float tanhApprox(float x)
  {
    if(x < -3)
      return -1;
    else if(x > 3)
      return 1;
    else
      return x * (27 + x * x) / (27 + 9 * x * x);
  }

  void setCutoff(float c)
  {
    cutoff = c;

    double fc  = cutoff / SOUND_SAMPLING_RATE_EXACT;
    double f   = fc * 0.5;
    double fc2 = fc * fc;
    double fc3 = fc * fc * fc;

    double fcr = 1.8730 * fc3 + 0.4955 * fc2 - 0.6490 * fc + 0.9988;
    acr        = -3.9364 * fc2 + 1.8409 * fc + 0.9968;

    tune = (1.0 - exp(-((2 * PI) * f * fcr))) / thermal;

    setResonance(resonance);
  }

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

      for(int j = 0; j < 2; j++)
      {
        float stan_temp = tanhApprox(curr_sample - resQuad * d5) * thermal;
        d0              = d0 + tune * (stan_temp - stan0);

        stan0 = tanhApprox(d0 * thermal);
        d1    = d1 + tune * (stan0 - stan1);

        stan1 = tanhApprox(d1 * thermal);
        d2    = d2 + tune * (stan1 - stan2);

        stan2 = tanhApprox(d2 * thermal);
        d3    = d3 + tune * (stan2 - tanhApprox(d3 * thermal));

        d5 = (d3 + d4) * 0.5;
        d4 = d3;
      }

      block->data[i] = to_int16(d5);
    }

    transmit(block);
    release(block);
  }

private:
  sound_block_t *inputQueueArray[1];

  double cutoff;
  double resonance;

  double stage[4];
  double stageTanh[3];
  double delay[6];

  double s0, s1, s2, s3;
  double stan0, stan1, stan2;
  double d0, d1, d2, d3, d4, d5;

  double thermal;
  double tune;
  double acr;
  double resQuad;
};
} // namespace Jamstack

#endif
