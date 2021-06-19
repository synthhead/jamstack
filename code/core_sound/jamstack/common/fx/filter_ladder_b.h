#pragma once

#ifndef filter_ladder_b_h_
#define filter_ladder_b_h_

#include <SoundStream.h>
#include <arm_math.h>

namespace Jamstack
{
class SoundFilterLadderB : public SoundStream
{
public:
  SoundFilterLadderB(void) : SoundStream(1, inputQueueArray), thermal(0.000025)
  {
    setCutoff(1000.0f);
    setResonance(0.1f);

    z1 = 0;
    z2 = 0;
    z3 = 0;
    z4 = 0;
    z5 = 0;
    y4 = 0;
    mf = 0;
  }

  void setResonance(float r)
  {
    resonance = r;
    resQuad   = 4.0 * resonance * acr;
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

  float tanhApprox(float x)
  {
    if(x < -3)
      return -1;
    else if(x > 3)
      return 1;
    else
      return x * (27 + x * x) / (27 + 9 * x * x);
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

    block = receiveWritable();

    if(!block)
      return;

    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      float sig = 0;

      float y1, y2;

      y1 = z1 + tune * (tanh((sig - 4. * resonance * mf * acr) / thermal) - tanh(z1 - thermal));
      z1 = y1;
      y2 = z2 + tune * (tanh(y1 / thermal) - tanh(z2 / thermal));
      z2 = y2;
    }

    transmit(block);
    release(block);
  }

private:
  sound_block_t *inputQueueArray[1];

  float cutoff;
  float resonance;

  float z1, z2, z3, z4, z5, y4, mf;

  float thermal;
  float tune;
  float acr;
  float resQuad;
};
} // namespace Jamstack

#endif
