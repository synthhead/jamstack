#ifndef JAMSTACK_TESTSEQ
#define JAMSTACK_TESTSEQ

#include "fsl_debug_console.h"
#include "central.h"
#include "clock.h"

struct SeqStep
{
  uint8_t pitch;
  uint8_t velocity;
};

class Testseq
{
public:
  Testseq() {}
  ~Testseq() {}

  void Init()
  {
    steps[0] = {.pitch = 0, .velocity = 127};
    steps[1] = {.pitch = 3, .velocity = 127};
    steps[2] = {.pitch = 8, .velocity = 127};
    steps[3] = {.pitch = 5, .velocity = 127};
    steps[4] = {.pitch = 7, .velocity = 127};
    steps[5] = {.pitch = 19, .velocity = 127};
    steps[6] = {.pitch = 7, .velocity = 127};
    steps[7] = {.pitch = 19, .velocity = 127};
  }

  void Start()
  {
    if(!is_running)
    {
      is_running = true;
      clock.onStart();
    }
  }
  void Stop()
  {
    if(is_running)
    {
      is_running = false;
      clock.onStop();
    }
  }
  void ToggleRunning()
  {
    if(is_running)
    {
      Stop();
      core_sound::central.NoteOff(0U, steps[last_bar_count].pitch + 36, steps[last_bar_count].velocity);
    }
    else
    {
      bar_count       = 0;
      beat_count      = 0;
      last_bar_count  = 7;
      last_beat_count = 7;
      Start();
    }
  }
  void UpdateState();
  void Tempo(uint16_t tempo)
  {
    uint32_t new_tempo = (uint32_t)new_map(tempo, 0, 64, 120000, 6000);
    PRINTF("Tempo: %i \r\n", new_tempo);
    clock.setStepLength(new_tempo);
  }

  static float new_map(float x, float in_min, float in_max, float out_min, float out_max)
  {
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min) + 0.5f;
  }
  void StartRecording()
  {
    is_recording = true;
    PRINTF("START REC \r\n");
  }
  void StopRecording()
  {
    is_recording = false;
    PRINTF("STOP REC \r\n");
  }

  Clock clock;

private:
  bool    is_running      = false;
  uint8_t bar_count       = 0;
  uint8_t beat_count      = 0;
  uint8_t last_bar_count  = 0;
  uint8_t last_beat_count = 0;
  bool    is_recording    = false;
  SeqStep steps[16];
};

extern Testseq testseq;

#endif /* JAMSTACK_TESTSEQ */
