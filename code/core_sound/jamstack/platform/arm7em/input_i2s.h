#ifndef _input_i2s_h_
#define _input_i2s_h_

#include "SoundStream.h"
#include "DMAChannel.h"

class SoundInputI2S : public SoundStream
{
public:
  SoundInputI2S(void) : SoundStream(0, NULL) { begin(); }
  virtual void update(void);
  void         begin(void);

protected:
  static bool       update_responsibility;
  static DMAChannel dma;
  static void       isr(void);

private:
  static sound_block_t* block_left;
  static sound_block_t* block_right;
  static uint16_t       block_offset;
};

#endif
