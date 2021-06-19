#include "effect_wavefolder.h"

void SoundEffectWavefolder::update()
{
  sound_block_t *blocka = receiveWritable(0);
  if(!blocka)
    return;
  sound_block_t *blockb = receiveReadOnly(1);
  if(!blockb)
  {
    release(blocka);
    return;
  }
  int16_t *pa = blocka->data;
  int16_t *pb = blockb->data;
  for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
  {
    int32_t a12 = pa[i];
    int32_t b12 = pb[i];

    int32_t s1 = (a12 * b12 + 0x400) >> 11;

    bool flip1 = ((s1 + 0x8000) >> 16) & 1;

    s1 = 0xFFFF & (flip1 ? ~s1 : +s1);

    pa[i] = s1;
  }
  transmit(blocka);
  release(blocka);
  release(blockb);
}
