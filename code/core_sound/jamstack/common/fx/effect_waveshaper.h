#ifndef JAMSTACK_EFFECT_WAVESHAPER
#define JAMSTACK_EFFECT_WAVESHAPER

#include "SoundStream.h"

class SoundEffectWaveshaper : public SoundStream
{
public:
  SoundEffectWaveshaper(void) : SoundStream(1, inputQueueArray) {}
  ~SoundEffectWaveshaper();
  virtual void update(void);
  void         shape(float *waveshape, int length);

private:
  sound_block_t *inputQueueArray[1];
  int16_t *      waveshape;
  int16_t        lerpshift;
};

#endif /* JAMSTACK_EFFECT_WAVESHAPER */
