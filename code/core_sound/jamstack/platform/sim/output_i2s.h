#ifndef output_i2s_sim_h_
#define output_i2s_sim_h_

#include "SoundStream.h"

class SoundOutputI2S : public SoundStream
{
public:
  SoundOutputI2S(void) : SoundStream(2, inputQueueArray) { begin(); }
  virtual void update(void);
  void         begin(void);
  friend class SoundInputI2S;
  static void isr(float buffer[]);

protected:
  static sound_block_t *block_left_1st;
  static sound_block_t *block_right_1st;
  static bool           update_responsibility;

private:
  static sound_block_t *block_left_2nd;
  static sound_block_t *block_right_2nd;
  static uint16_t       block_left_offset;
  static uint16_t       block_right_offset;
  sound_block_t *       inputQueueArray[2];
};

#endif
