#ifndef JAMSTACK_CLOCK_H_
#define JAMSTACK_CLOCK_H_

#include <stdint.h>

enum class ClockMode
{
  INTERNAL_CLOCK,
  MIDI_CLOCK,
  TRIGGER
};

class Clock
{
public:
  void  setSwing(float newSwing) { swing = newSwing; };
  float getSwing() { return swing; };

  void     onStart();
  void     onStop();
  void     notifyMidiClockReceived() { midiClockReceived = true; }
  void     changeStepLength(float factor);
  void     setStepLength(uint32_t newStepLength);
  uint32_t getStepLength() { return stepLength; };

  void setClockMode(ClockMode newClockMode);
  void onTriggerReceived() { triggerReceived = true; };
  bool update();

  uint8_t   getStepCount() { return stepCount; }
  ClockMode getClockMode() { return clockMode; }

private:
  bool shouldStepMidiClock();
  bool shouldStepInternalClock();
  bool shouldStepTriggerInput();
  bool midiClockStepped = false;

  ClockMode clockMode;
  uint8_t   stepCount    = 0;
  uint32_t  lastStepTime = 0;
  uint32_t  nextStepTime = 0;
  uint32_t  stepLength   = 120000;

  int8_t   pulseCount            = -1;
  float    swing                 = 0.0;
  uint32_t swingAbsolute         = 0;
  bool     midiClockReceived     = false;
  uint8_t  previousTriggerSignal = 1;
  bool     triggerReceived       = false;
};

#endif /* JAMSTACK_CLOCK_H_ */
