#ifndef effect_delay_h_
#define effect_delay_h_

#include "SoundStream.h"
#include "utils/dspinst.h"

#define DELAY_QUEUE_SIZE (176512 / SOUND_BLOCK_SAMPLES)

class SoundFxDelay : public SoundStream
{
public:
  SoundFxDelay() : SoundStream(1, inputQueueArray)
  {
    activemask = 0;
    headindex  = 0;
    tailindex  = 0;
    maxblocks  = 0;
    memset(queue, 0, sizeof(queue));
  }
  void delay(uint8_t channel, float milliseconds)
  {
    if(channel >= 8)
      return;
    if(milliseconds < 0.0)
      milliseconds = 0.0;
    uint32_t n    = (milliseconds * (SOUND_SAMPLING_RATE_EXACT / 1000.0)) + 0.5;
    uint32_t nmax = SOUND_BLOCK_SAMPLES * (DELAY_QUEUE_SIZE - 1);
    if(n > nmax)
      n = nmax;
    uint32_t blks = (n + (SOUND_BLOCK_SAMPLES - 1)) / SOUND_BLOCK_SAMPLES + 1;
    if(!(activemask & (1 << channel)))
    {
      position[channel] = n;
      if(blks > maxblocks)
        maxblocks = blks;
      activemask |= (1 << channel);
    }
    else
    {
      if(n > position[channel])
      {
        if(blks > maxblocks)
          maxblocks = blks;
        position[channel] = n;
      }
      else
      {
        position[channel] = n;
        recompute_maxblocks();
      }
    }
  }
  void disable(uint8_t channel)
  {
    if(channel >= 8)
      return;

    activemask &= ~(1 << channel);

    recompute_maxblocks();
  }
  virtual void update(void) override;

private:
  void recompute_maxblocks(void)
  {
    uint32_t max     = 0;
    uint32_t channel = 0;
    do
    {
      if(activemask & (1 << channel))
      {
        uint32_t n = position[channel];
        n          = (n + (SOUND_BLOCK_SAMPLES - 1)) / SOUND_BLOCK_SAMPLES + 1;
        if(n > max)
          max = n;
      }
    } while(++channel < 8);
    maxblocks = max;
  }
  uint8_t  activemask;
  uint16_t headindex;
  uint16_t tailindex;
  uint16_t maxblocks;
#if DELAY_QUEUE_SIZE * SOUND_BLOCK_SAMPLES < 65535
  uint16_t position[8];
#else
  uint32_t position[8];
#endif
  sound_block_t *queue[DELAY_QUEUE_SIZE];
  sound_block_t *inputQueueArray[1];
};

#endif
