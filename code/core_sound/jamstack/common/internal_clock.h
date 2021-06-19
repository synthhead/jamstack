#ifndef JAMSTACK_INTERNAL_CLOCK_H_
#define JAMSTACK_INTERNAL_CLOCK_H_

#include "jamstackcommons.h"

namespace jamstack
{
//
class InternalClock
{
public:
  InternalClock()
  {
    phase_           = 0;
    phase_increment_ = 0;
  }

  ~InternalClock() {}

  inline void Start(uint32_t tempo, uint8_t swing)
  {
    phase_      = 0;
    swing_step_ = 11;
    set_tempo(tempo);
    set_swing(swing);
  }

  inline void set_tempo(uint32_t tempo)
  {
    phase_increment_ = 178957UL * tempo / 10; // For 48kHz
    // phase_increment_ = 128849UL * tempo / 6;  // For 40kHz
  }

  inline void set_swing(uint32_t swing) { swing_amount_ = swing * (0x80000000 / 3 / 100); }

  inline bool Process()
  {
    uint32_t half_cycle = 0x80000000;
    if(swing_step_ < 6)
    {
      half_cycle += swing_amount_;
    }
    else
    {
      half_cycle -= swing_amount_;
    }

    bool tick = false;
    if(phase_ >= half_cycle)
    {
      tick = true;
      phase_ -= half_cycle;
      ++swing_step_;
      if(swing_step_ >= 12)
      {
        swing_step_ = 0;
      }
    }
    phase_ += phase_increment_;
    return tick;
  }

private:
  uint32_t phase_;
  uint32_t phase_increment_;
  uint32_t swing_amount_;
  uint8_t  swing_step_;

  DISALLOW_COPY_AND_ASSIGN(InternalClock);
};

} // namespace jamstack

#endif // JAMSTACK_INTERNAL_CLOCK_H_
