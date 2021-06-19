#pragma once

#ifndef filter_ladder_d_h
#define filter_ladder_d_h

#include <SoundStream.h>
#include <arm_math.h>

namespace Jamstack
{
class SoundFilterLadderD : public SoundStream
{
public:
  SoundFilterLadderD(void) : SoundStream(1, inputQueueArray), srate(44100.f), oversampling(1.f)
  {
    fs        = srate * oversampling;
    cutoff    = 0.1f;
    resonance = 0.7f;
    updateFilterParams();
  }

  void updateFilterParams(void)
  {
    k = resonance * 120.f;

    fc = .5f * srate * exp((1 - cutoff) * log(20.f / 22050.f));
    g  = tan(M_PI / fs * fc) / sqrt(1.f + k);

    VT2  = 0.052f;
    VT2i = 19.23076923076923f;

    p0s = 1.0f / (1.0f + g);
    q0s = 1.0f - g;
    r1s = -g;
    k0s = VT2 * g * p0s;

    nmp = (1.0f - p0s);
    gN  = nmp * nmp;
    kgN = k * gN;
    p0g = 1.0f / (1.0f + kgN);
    k0g = -VT2i * p0g;

    rg1 = -2.0f * kgN;
    rg2 = -1.0f * kgN;

    acc = tmp = p0s * (g - 1.0f);

    qg1 = -2.0f * (kgN + acc);
    acc = acc * tmp;
    qg2 = -1.0f * (kgN + acc);
  }

  void setResonance(float r)
  {
    resonance = r;
    updateFilterParams();
  }
  void setCutoff(float freq)
  {
    cutoff = freq;
    updateFilterParams();
  }

  int16_t to_int16(float f)
  {
    if(f > 1.0)
      f = 1.0f;
    if(f < -1.0)
      f = -1.0f;
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
      float curr_sample = (block->data[i] / 32767.0f) * VT2;
      float frac        = 0.5f;

      yo = tanh(k0g * (curr_sample + sg1));
      A  = yo;

      yi = yo;
      yd = k0s * (yi + sf1);
      y  = yd + si1;
      yo = tanh(VT2i * y);
      B  = yo;

      si1 = yd + y;
      sf1 = r1s * yi - q0s * yo;

      yi = yo;
      yd = k0s * (yi + sf2);
      y  = yd + si2;
      yo = tanh(VT2i * y);

      si2 = yd + y;
      sf2 = r1s * yi - q0s * yo;

      yf = k * y;

      sg1 = rg1 * curr_sample + qg1 * yf + sg2;
      sg2 = rg2 * curr_sample + qg2 * yf;

      f1 = y * (1.f + k);
      f2 = VT2 * (2.f * B - 2.f * yo) * 8;

      block->data[i] = to_int16((f2 * frac + f1 * (1.0 - frac)) * VT2i);
    }

    transmit(block);
    release(block);
  }

private:
  sound_block_t *inputQueueArray[1];

  float srate;
  float oversampling;

  float cutoff;
  float resonance;

  float fs, fc, g, p0s, nmp, gN, kgN, p0g, tmp, acc;

  float VT2, VT2i, k0g, k0s, q0s, r1s, k;
  float rg1, rg2;
  float qg1, qg2;
  float si1, sf1, sg1, si2, sf2, sg2;

  float yi, yd, yf;

  float A, B, C, D, y, yo;
  float f1, f2;
};
} // namespace Jamstack

#endif
