#include "synth_karplusstrong.h"

static uint32_t pseudorand(uint32_t lo)
{
  uint32_t hi;

  hi = multiply_16bx16t(16807, lo);
  lo = 16807 * (lo & 0xFFFF);
  lo += (hi & 0x7FFF) << 16;
  lo += hi >> 15;
  lo = (lo & 0x7FFFFFFF) + (lo >> 31);
  return lo;
}

void SoundSynthKarplusStrong::update(void)
{
  sound_block_t *block;

  if(state == 0)
    return;

  if(state == 1)
  {
    uint32_t lo = seed;
    for(int i = 0; i < bufferLen; i++)
    {
      lo        = pseudorand(lo);
      buffer[i] = signed_multiply_32x16b(magnitude, lo);
    }
    seed  = lo;
    state = 2;
  }

  block = allocate();
  if(!block)
  {
    state = 0;
    return;
  }

  int16_t prior;
  if(bufferIndex > 0)
  {
    prior = buffer[bufferIndex - 1];
  }
  else
  {
    prior = buffer[bufferLen - 1];
  }
  int16_t *data = block->data;
  for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
  {
    int16_t in = buffer[bufferIndex];

    int16_t out = (in * 32686 + prior * 32686) >> 16;

    *data++             = out;
    buffer[bufferIndex] = out;
    prior               = in;
    if(++bufferIndex >= bufferLen)
      bufferIndex = 0;
  }

  transmit(block);
  release(block);
}

uint32_t SoundSynthKarplusStrong::seed = 1;
