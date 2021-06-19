#ifndef mixer_h_
#define mixer_h_

#include "SoundStream.h"

class SoundMixer4 : public SoundStream
{
public:
  SoundMixer4(void) : SoundStream(4, inputQueueArray)
  {
    for(int i = 0; i < 4; i++)
      multiplier[i] = 65536; // == 1.0
  }

  virtual void update(void) override;

  void gain(unsigned int channel, float gain)
  {
    if(channel >= 4)
      return;

    if(gain > 32767.0f)
      gain = 32767.0f;
    else if(gain < -32767.0f)
      gain = -32767.0f;

    multiplier[channel] = gain * 65536.0f; // TODO: proper roundoff?
  }

private:
  int32_t        multiplier[4];
  sound_block_t* inputQueueArray[4];
};

class SoundAmplifier : public SoundStream
{
public:
  SoundAmplifier(void) : SoundStream(1, inputQueueArray), multiplier(65536) {}

  virtual void update(void) override;

  void gain(float n)
  {
    if(n > 32767.0f)
      n = 32767.0f;
    else if(n < -32767.0f)
      n = -32767.0f;

    multiplier = n * 65536.0f;
  }

private:
  int32_t        multiplier;
  sound_block_t* inputQueueArray[1];
};

#endif
