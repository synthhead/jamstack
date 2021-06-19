#ifndef synth_karplusstrong_h_
#define synth_karplusstrong_h_

#include "SoundStream.h"
#include "utils/dspinst.h"

class SoundSynthKarplusStrong : public SoundStream
{
public:
  SoundSynthKarplusStrong() : SoundStream(0, NULL) { state = 0; }
  void noteOn(float frequency, float velocity)
  {
    if(velocity > 1.0f)
    {
      velocity = 0.0f;
    }
    else if(velocity <= 0.0f)
    {
      noteOff(1.0f);
      return;
    }
    magnitude = velocity * 65535.0f;
    int len   = (SOUND_SAMPLING_RATE_EXACT / frequency) + 0.5f;
    if(len > 536)
      len = 536;
    bufferLen   = len;
    bufferIndex = 0;
    state       = 1;
  }

  void         noteOff(float velocity) { state = 0; }
  virtual void update(void) override;

private:
  uint8_t         state;
  uint16_t        bufferLen;
  uint16_t        bufferIndex;
  int32_t         magnitude;
  static uint32_t seed;
  int16_t         buffer[536];
};

#endif
