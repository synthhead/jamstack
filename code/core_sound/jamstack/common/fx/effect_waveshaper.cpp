#include "effect_waveshaper.h"

SoundEffectWaveshaper::~SoundEffectWaveshaper()
{
  if(this->waveshape)
  {
    delete[] this->waveshape;
  }
}

void SoundEffectWaveshaper::shape(float *waveshape, int length)
{
  if(!waveshape || length < 2 || length > 32769 || ((length - 1) & (length - 2)))
    return;

  if(this->waveshape)
  {
    delete[] this->waveshape;
  }
  this->waveshape = new int16_t[length];
  for(int i = 0; i < length; i++)
  {
    this->waveshape[i] = 32767 * waveshape[i];
  }

  int index = length - 1;
  lerpshift = 16;
  while(index >>= 1)
    --lerpshift;
}

void SoundEffectWaveshaper::update(void)
{
  if(!waveshape)
    return;

  sound_block_t *block;
  block = receiveWritable();
  if(!block)
    return;

  uint16_t x, xa;
  int16_t  i, ya, yb;
  for(i = 0; i < SOUND_BLOCK_SAMPLES; i++)
  {
    x = block->data[i] + 32768;

    xa             = x >> lerpshift;
    ya             = waveshape[xa];
    yb             = waveshape[xa + 1];
    block->data[i] = ya + ((yb - ya) * (x - (xa << lerpshift)) >> lerpshift);
  }

  transmit(block);
  release(block);
}
