#ifndef com_basicsynth_h
#define com_basicsynth_h

#include "SoundStream.h"
#include "jamstackcommons.h"

#include "fx/effect_envelope.h"
#include "fx/effect_delay.h"
#include "fx/filter_variable.h"
#include "fx/filter_ladder_c.h"
#include "fx/filter_ladder_d.h"

#include "synth/synth_sine.h"
#include "synth/synth_variable_saw.h"
#include "synth/synth_blosc.h"
#include "mixer.h"

#include "com_patcher.h"

const uint8_t numCords = 16;

class BasicSynth
{
public:
  BasicSynth();
  ~BasicSynth();
  SoundMixer4 *output;

  enum Engines
  {
    BASIC,
    VAR_SAW,
    BLOSC
  };

  void onNoteOn(int note, int velocity);
  void onNoteOff(int note);
  void onControlChange(int id, int value);

  void disableFilter();
  void enableFilter();

  void toggleBlosc();
  void toggleVarSaw();
  void toggleCheapOsc();

  void changeEngine(int8_t engine);

private:
  SoundFxEnvelope *         adsr1;
  SoundFxDelay *            delay1;
  SoundMixer4 *             delayPremix;
  SoundFilterStateVariable *svf1;

  Jamstack::SoundSynthWaveformSine *sine1;
  SoundSynthVariableSaw *           saw1;
  SoundSynthBlosc *                 blosc;
  Jamstack::SoundFilterLadderC *    filterLadderC;
  Jamstack::SoundFilterLadderD *    filterLadderD;

  Jamstack::Patcher *patcher1;

  SoundConnection *cords;
  SoundConnection *cordOscToFilter;
  SoundConnection *cordMainOut;
};

#endif
