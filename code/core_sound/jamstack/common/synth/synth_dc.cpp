#include "synth_dc.h"

void SoundSynthWaveformDc::update(void)
{
  sound_block_t *block;
  uint32_t *     p, *end, val;
  int32_t        count, t1, t2, t3, t4;

  block = allocate();
  if(!block)
    return;
  p   = (uint32_t *)(block->data);
  end = p + SOUND_BLOCK_SAMPLES / 2;

  if(state == 0)
  {
    val = pack_16t_16t(magnitude, magnitude);
    do
    {
      *p++ = val;
      *p++ = val;
      *p++ = val;
      *p++ = val;
      *p++ = val;
      *p++ = val;
      *p++ = val;
      *p++ = val;
    } while(p < end);
  }
  else
  {
    count = substract_int32_then_divide_int32(target, magnitude, increment);
    if(count >= SOUND_BLOCK_SAMPLES)
    {
      do
      {
        magnitude += increment;
        t1 = magnitude;
        magnitude += increment;
        t1 = pack_16t_16t(magnitude, t1);
        magnitude += increment;
        t2 = magnitude;
        magnitude += increment;
        t2 = pack_16t_16t(magnitude, t2);
        magnitude += increment;
        t3 = magnitude;
        magnitude += increment;
        t3 = pack_16t_16t(magnitude, t3);
        magnitude += increment;
        t4 = magnitude;
        magnitude += increment;
        t4   = pack_16t_16t(magnitude, t4);
        *p++ = t1;
        *p++ = t2;
        *p++ = t3;
        *p++ = t4;
      } while(p < end);
    }
    else
    {
      while(count >= 2)
      {
        count -= 2;
        magnitude += increment;
        t1 = magnitude;
        magnitude += increment;
        t1   = pack_16t_16t(magnitude, t1);
        *p++ = t1;
      }
      if(count)
      {
        t1   = pack_16t_16t(target, magnitude + increment);
        *p++ = t1;
      }
      magnitude = target;
      state     = 0;
      val       = pack_16t_16t(magnitude, magnitude);
      while(p < end)
      {
        *p++ = val;
      }
    }
  }
  transmit(block);
  release(block);
}
