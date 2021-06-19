#pragma once

#ifndef filter_ladder_c_h
#define filter_ladder_c_h

#include <SoundStream.h>
#include <arm_math.h>

namespace Jamstack
{
class SoundFilterLadderC : public SoundStream
{
public:
  SoundFilterLadderC(void) : SoundStream(1, inputQueueArray), oversampling(1.f), srate(44100.f)
  {
    cutoff    = 0.5f;
    resonance = 0.5f;
    updateFilterParams();
  }

  void setCutoff(float freq)
  {
    cutoff = freq;
    updateFilterParams();
  }
  void setResonance(float r)
  {
    resonance = r;
    updateFilterParams();
  }
  void updateFilterParams(void)
  {
    k  = resonance * 3.9999999999999987f;
    fc = .5f * srate * exp((1.f - cutoff) * log(20.f / 22050.f));
    fs = srate * fmax(oversampling, 1.f);
    fc = fmin(fc, fs / 8.f);

    g    = tan(M_PI * fc / fs) / sqrt(1.0f + sqrt(k) - 2.f * pow(k, 0.25f) * 0.7071067811865476f);
    VT2  = 0.052f;
    VT2i = 19.23076923076923f;

    p0s = 1.0f / (1.0f + g);
    q0s = 1.0f - g;
    r1s = -g;
    k0s = VT2 * g * p0s;

    nmp = (1.0f - p0s);
    gN  = nmp * nmp * nmp * nmp;
    kgN = k * gN;
    p0g = 1.0f / (1.0f + kgN);
    k0g = -VT2i * p0g;

    rg1 = -4.0f * kgN;
    rg2 = -6.0f * kgN;
    rg3 = -4.0f * kgN;
    rg4 = -1.0f * kgN;

    acc = tmp = p0s * (g - 1.0f);

    qg1 = -4.0f * (kgN + acc);
    acc = acc * tmp;
    qg2 = -6.0f * (kgN + acc);
    acc = acc * tmp;
    qg3 = -4.0f * (kgN + acc);
    acc = acc * tmp;
    qg4 = -1.0f * (kgN + acc);
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

      curr_sample *= VT2;
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
      C  = yo;

      si2 = yd + y;
      sf2 = r1s * yi - q0s * yo;

      yi = yo;
      yd = k0s * (yi + sf3);
      y  = yd + si3;
      yo = tanh(VT2i * y);
      D  = yo;

      si3 = yd + y;
      sf3 = r1s * yi - q0s * yo;

      yi = yo;
      yd = k0s * (yi + sf4);
      y  = yd + si4;
      yo = tanh(VT2i * y);

      si4 = yd + y;
      sf4 = r1s * yi - q0s * yo;
      yf  = k * y;

      sg1 = rg1 * curr_sample + qg1 * yf + sg2;
      sg2 = rg2 * curr_sample + qg2 * yf + sg3;
      sg3 = rg3 * curr_sample + qg3 * yf + sg4;
      sg4 = rg4 * curr_sample + qg4 * yf;

      block->data[i] = to_int16((y * (1.f + k) * VT2i) * 0.9f);
    }

    transmit(block);
    release(block);
  }

private:
  sound_block_t *inputQueueArray[1];

  float cutoff;
  float resonance;
  float oversampling;

  float fs, fc, g, p0s, nmp, gN, kgN, p0g, tmp, acc;

  float srate;

  float VT2, VT2i, k0g, k0s, q0s, r1s, k;
  float rg1, rg2, rg3, rg4;
  float qg1, qg2, qg3, qg4;
  float si1, sf1, sg1, si2, sf2, sg2, si3, sf3, sg3, si4, sf4, sg4;

  float yi, yd, yf;

  float A, B, C, D, y, yo;
};
} // namespace Jamstack

#endif
