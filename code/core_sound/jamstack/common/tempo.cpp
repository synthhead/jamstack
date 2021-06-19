#include "tempo.h"

namespace jamstack
{
//
void Tempo::Init()
{
  running_   = false;
  latched_   = false;
  recording_ = false;

  settings_.clock_tempo           = 120;
  settings_.clock_swing           = 0;
  settings_.clock_input_division  = 1;
  settings_.clock_output_division = 7;
  settings_.clock_bar_duration    = 4;
  settings_.clock_override        = 0;
  settings_.nudge_first_tick      = 0;
  settings_.clock_manual_start    = 0;
}

void Tempo::Refresh()
{
  if(clock_pulse_counter_)
    --clock_pulse_counter_;

  if(reset_pulse_counter_)
    --reset_pulse_counter_;
}

Tempo tempo;

} // namespace jamstack
