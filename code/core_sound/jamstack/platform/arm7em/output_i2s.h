#ifndef output_i2s_h_
#define output_i2s_h_

#ifndef I2S_WORD_SIZE
#define I2S_WORD SIZE 16
#endif

#include "SoundStream.h"
#include "DMAChannel.h"

class SoundOutputI2S : public SoundStream
{
public:
  SoundOutputI2S(void) : SoundStream(2, inputQueueArray) { begin(); }
  virtual void update(void);
  void         begin(void);
  friend class SoundInputI2S;

protected:
  static void           config_i2s(void);
  static sound_block_t *block_left_1st;
  static sound_block_t *block_right_1st;
  static bool           update_responsibility;
  static DMAChannel     dma;
  static void           isr(void);

private:
  static sound_block_t *block_left_2nd;
  static sound_block_t *block_right_2nd;
  static uint16_t       block_left_offset;
  static uint16_t       block_right_offset;
  sound_block_t *       inputQueueArray[2];
};

#endif
