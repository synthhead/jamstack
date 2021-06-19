#include "synth_noisewhite.h"

void SoundSynthNoiseWhite::update(void)
{
  sound_block_t *block;
  uint32_t *     p, *end;
  int32_t        n1, n2, gain;
  uint32_t       lo, hi, val1, val2;

  gain = level;
  if(gain == 0)
    return;
  block = allocate();
  if(!block)
    return;
  p   = (uint32_t *)(block->data);
  end = p + SOUND_BLOCK_SAMPLES / 2;
  lo  = seed;
  do
  {
#if defined(__ARM_ARCH_7EM__)
    hi = multiply_16bx16t(16807, lo);
    lo = 16807 * (lo & 0xFFFF);
    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;
    lo = (lo & 0x7FFFFFFF) + (lo >> 31);
    n1 = signed_multiply_32x16b(gain, lo);
    hi = multiply_16bx16t(16807, lo);
    lo = 16807 * (lo & 0xFFFF);
    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;
    lo   = (lo & 0x7FFFFFFF) + (lo >> 31);
    n2   = signed_multiply_32x16b(gain, lo);
    val1 = pack_16b_16b(n2, n1);
    hi   = multiply_16bx16t(16807, lo);
    lo   = 16807 * (lo & 0xFFFF);
    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;
    lo = (lo & 0x7FFFFFFF) + (lo >> 31);
    n1 = signed_multiply_32x16b(gain, lo);
    hi = multiply_16bx16t(16807, lo);
    lo = 16807 * (lo & 0xFFFF);
    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;
    lo   = (lo & 0x7FFFFFFF) + (lo >> 31);
    n2   = signed_multiply_32x16b(gain, lo);
    val2 = pack_16b_16b(n2, n1);
    *p++ = val1;
    *p++ = val2;
#else
    hi = 16807 * (lo >> 16);
    lo = 16807 * (lo & 0xFFFF);
    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;
    lo = (lo & 0x7FFFFFFF) + (lo >> 31);
    n1 = signed_multiply_32x16b(gain, lo);
    hi = 16807 * (lo >> 16);
    lo = 16807 * (lo & 0xFFFF);
    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;
    lo   = (lo & 0x7FFFFFFF) + (lo >> 31);
    n2   = signed_multiply_32x16b(gain, lo);
    val1 = pack_16b_16b(n2, n1);
    *p++ = val1;
#endif
  } while(p < end);
  seed = lo;
  transmit(block);
  release(block);
}

uint16_t SoundSynthNoiseWhite::instance_count = 0;
