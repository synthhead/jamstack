#ifndef JAMSTACK_TEMPO_H_
#define JAMSTACK_TEMPO_H_

#include "jamstackcommons.h"
#include "internal_clock.h"

namespace jamstack
{
//
struct TempoSettings
{
  uint8_t clock_tempo;
  uint8_t clock_swing;
  uint8_t clock_input_division;
  uint8_t clock_output_division;
  uint8_t clock_bar_duration;
  uint8_t clock_override;
  uint8_t nudge_first_tick;
  uint8_t clock_manual_start;
};

class Tempo
{
public:
  Tempo() {}
  ~Tempo() {}

  void Init();

  void Clock();

  void Touch();
  void Refresh();
  void RefreshInternalClock()
  {
    if(running() && internal_clock() && internal_clock_.Process())
    {
      ++internal_clock_ticks_;
    }
  }
  void ProcessInternalClockEvents()
  {
    while(internal_clock_ticks_)
    {
      // Clock(); //! Temp. Does not compile!
      --internal_clock_ticks_;
    }
  }

  inline bool clock() const
  {
    return clock_pulse_counter_ > 0 && (!settings_.nudge_first_tick || settings_.clock_bar_duration == 0 || !reset());
  }
  inline bool    reset() const { return reset_pulse_counter_ > 0; }
  inline bool    internal_clock() const { return settings_.clock_tempo >= 40; }
  inline uint8_t tempo() const { return settings_.clock_tempo; }
  inline bool    running() const { return running_; }

private:
  TempoSettings settings_;

  bool running_;
  bool started_by_keyboard_; // todo
  bool latched_;
  bool recording_;

  InternalClock internal_clock_;
  uint8_t       internal_clock_ticks_;
  uint16_t      midi_clock_tick_duration_;

  int16_t swing_predelay_[12];
  uint8_t swing_counter_;

  uint8_t  clock_input_prescaler_;
  uint8_t  clock_output_prescaler_;
  uint16_t bar_position_;
  uint8_t  stop_count_down_;

  uint16_t clock_pulse_counter_;
  uint16_t reset_pulse_counter_;

  uint16_t previous_output_division_;
  bool     needs_resync_;

  DISALLOW_COPY_AND_ASSIGN(Tempo);
};

extern Tempo tempo;

} // namespace jamstack
#endif /* JAMSTACK_TEMPO_H_ */
