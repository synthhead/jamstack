#ifndef JAMSTACK_EFFECT_WAVEFOLDER
#define JAMSTACK_EFFECT_WAVEFOLDER

#include "SoundStream.h"

class SoundEffectWavefolder : public SoundStream
{
public:
  SoundEffectWavefolder() : SoundStream(2, inputQueueArray) {}
  virtual void update();

private:
  sound_block_t *inputQueueArray[2];
};

#endif
