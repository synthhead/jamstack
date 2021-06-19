#include "synth_sine.h"
#include "utils/dspinst.h"
#include "../data/data_waveforms.h"

using namespace Jamstack;

void SoundSynthWaveformSine::update(void)
{
  sound_block_t *block;
  uint32_t       i, ph, inc, index, scale;
  int32_t        val1, val2;

  if(magnitude)
  {
    block = allocate();
    if(block)
    {
      ph  = phase_accumulator;
      inc = phase_increment;
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i++)
      {
        index = ph >> 24;
        val1  = SoundWaveSine[index];
        val2  = SoundWaveSine[index + 1];
        scale = (ph >> 8) & 0xFFFF;
        val2 *= scale;
        val1 *= 0x10000 - scale;

        block->data[i] = multiply_32x32_rshift32(val1 + val2, magnitude);

        ph += inc;
      }
      phase_accumulator = ph;
      transmit(block);
      release(block);
      return;
    }
  }
  phase_accumulator += phase_increment * SOUND_BLOCK_SAMPLES;
}

static int32_t taylor(uint32_t ph)
{
  int32_t angle, sum, p1, p2, p3, p5, p7, p9, p11;

  if(ph >= 0xC0000000 || ph < 0x40000000)
  {
    angle = (int32_t)ph;
  }
  else
  {
    angle = (int32_t)(0x80000000u - ph);
  }
  p1  = multiply_32x32_rshift32_rounded(angle, 1686629713) << 2;
  p2  = multiply_32x32_rshift32_rounded(p1, p1) << 1;
  p3  = multiply_32x32_rshift32_rounded(p2, p1) << 2;
  sum = multiply_subtract_32x32_rshift32_rounded(p1, p3, 1431655765);
  p5  = multiply_32x32_rshift32_rounded(p3, p2);
  sum = multiply_accumulate_32x32_rshift32_rounded(sum, p5, 572662306);
  p7  = multiply_32x32_rshift32_rounded(p5, p2);
  sum = multiply_subtract_32x32_rshift32_rounded(sum, p7, 109078534);
  p9  = multiply_32x32_rshift32_rounded(p7, p2);
  sum = multiply_accumulate_32x32_rshift32_rounded(sum, p9, 12119837);
  p11 = multiply_32x32_rshift32_rounded(p9, p2);
  sum = multiply_subtract_32x32_rshift32_rounded(sum, p11, 881443);
  return sum <<= 1;
}

void SoundSynthWaveformSineHires::update(void)
{
  sound_block_t *msw, *lsw;
  uint32_t       i, ph, inc;
  int32_t        val;

  if(magnitude)
  {
    msw = allocate();
    lsw = allocate();
    if(msw && lsw)
    {
      ph  = phase_accumulator;
      inc = phase_increment;
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i++)
      {
        val          = taylor(ph);
        msw->data[i] = val >> 16;
        lsw->data[i] = val & 0xFFFF;
        ph += inc;
      }
      phase_accumulator = ph;
      transmit(msw, 0);
      release(msw);
      transmit(lsw, 1);
      release(lsw);
      return;
    }
    else
    {
      if(msw)
        release(msw);
      if(lsw)
        release(lsw);
    }
  }
  phase_accumulator += phase_increment * SOUND_BLOCK_SAMPLES;
}

void SoundSynthWaveformSineModulated::update(void)
{
  sound_block_t *block, *modinput;
  uint32_t       i, ph, inc, index, scale;
  int32_t        val1, val2;
  int16_t        mod;

  modinput = receiveReadOnly();
  ph       = phase_accumulator;
  inc      = phase_increment;
  block    = allocate();
  if(!block)
  {
    if(modinput)
    {
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i++)
      {
        mod = modinput->data[i];
        ph += inc + (multiply_32x32_rshift32(inc, mod << 16) << 1);
      }
      release(modinput);
    }
    else
    {
      ph += phase_increment * SOUND_BLOCK_SAMPLES;
    }
    phase_accumulator = ph;
    return;
  }
  if(modinput)
  {
    for(i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      index = ph >> 24;
      val1  = SoundWaveSine[index];
      val2  = SoundWaveSine[index + 1];
      scale = (ph >> 8) & 0xFFFF;
      val2 *= scale;
      val1 *= 0x10000 - scale;

      block->data[i] = multiply_32x32_rshift32(val1 + val2, magnitude);

      mod = modinput->data[i];
      ph += inc + (multiply_32x32_rshift32(inc, mod << 16) << 1);
    }
    release(modinput);
  }
  else
  {
    ph  = phase_accumulator;
    inc = phase_increment;
    for(i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      index = ph >> 24;
      val1  = SoundWaveSine[index];
      val2  = SoundWaveSine[index + 1];
      scale = (ph >> 8) & 0xFFFF;
      val2 *= scale;
      val1 *= 0x10000 - scale;
      block->data[i] = multiply_32x32_rshift32(val1 + val2, magnitude);
      ph += inc;
    }
  }
  phase_accumulator = ph;
  transmit(block);
  release(block);
}
