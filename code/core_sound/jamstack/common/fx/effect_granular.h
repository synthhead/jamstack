#ifndef JAMSTACK_EFFECT_GRANULAR
#define JAMSTACK_EFFECT_GRANULAR

#include "SoundStream.h"
#include "jamstackcommons.h"

class SoundFxGranular : public SoundStream
{
public:
  SoundFxGranular(void) : SoundStream(1, inputQueueArray) {}
  void begin(int16_t *sample_bank_def, int16_t max_len_def);
  void setSpeed(float ratio)
  {
    if(ratio < 0.125)
      ratio = 0.125;
    else if(ratio > 8.0)
      ratio = 8.0;
    playpack_rate = ratio * 65536.0 + 0.499;
  }
  void beginFreeze(float grain_length)
  {
    if(grain_length <= 0.0)
      return;
    beginFreeze_int(grain_length * (SOUND_SAMPLING_RATE_EXACT * 0.001) + 0.5);
  }
  void beginPitchShift(float grain_length)
  {
    if(grain_length <= 0.0)
      return;
    beginPitchShift_int(grain_length * (SOUND_SAMPLING_RATE_EXACT * 0.001) + 0.5);
  }
  void         stop();
  virtual void update(void);

private:
  void           beginFreeze_int(int grain_samples);
  void           beginPitchShift_int(int grain_samples);
  sound_block_t *inputQueueArray[1];
  int16_t *      sample_bank;
  uint32_t       playpack_rate;
  uint32_t       accumulator;
  int16_t        max_sample_len;
  int16_t        write_head;
  int16_t        read_head;
  int16_t        grain_mode;
  int16_t        freeze_len;
  int16_t        prev_input;
  int16_t        glitch_len;
  bool           allow_len_change;
  bool           sample_loaded;
  bool           write_en;
  bool           sample_req;
};

#endif
