#include "testseq.h"

void Testseq::UpdateState()
{
  bool step = clock.update();
  if(step)
  {
    if(is_running && last_bar_count != bar_count)
    {
      core_sound::central.NoteOff(0U, steps[last_bar_count].pitch + 36, steps[last_bar_count].velocity);
      core_sound::central.NoteOn(0U, steps[bar_count].pitch + 36, steps[bar_count].velocity);
    }

    // if(is_running && (last_beat_count != beat_count))
    // {
    //   core_sound::central.NoteOff(0U, last_beat_count + 36, 127U);
    //   core_sound::central.NoteOn(0U, beat_count + 36, 127U);
    // }

    // PRINTF("Seq boom! %i.%i \r\n", bar_count, beat_count);

    last_bar_count  = bar_count;
    last_beat_count = beat_count;

    beat_count++;

    if(beat_count > 7)
    {
      beat_count = 0;
      bar_count++;

      if(bar_count > 7)
      {
        bar_count = 0;
      }
    }
  }
}

Testseq testseq;
