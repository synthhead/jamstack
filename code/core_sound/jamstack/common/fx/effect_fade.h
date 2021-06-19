#ifndef effect_fade_h_
#define effect_fade_h_

#include "SoundStream.h"
#include "jamstackcommons.h"

class SoundFxFade : public SoundStream
{
public:
  SoundFxFade(void) : SoundStream(1, inputQueueArray), position(0xFFFFFFFF) {}
  void fadeIn(uint32_t milliseconds)
  {
    uint32_t samples = (uint32_t)(milliseconds * 441u + 5u) / 10u;

    fadeBegin(0xFFFFFFFFu / samples, 1);
  }
  void fadeOut(uint32_t milliseconds)
  {
    uint32_t samples = (uint32_t)(milliseconds * 441u + 5u) / 10u;

    fadeBegin(0xFFFFFFFFu / samples, 0);
  }
  virtual void update(void) override;

private:
  void           fadeBegin(uint32_t newrate, uint8_t dir);
  uint32_t       position;
  uint32_t       rate;
  uint8_t        direction;
  sound_block_t *inputQueueArray[1];
};

#endif
