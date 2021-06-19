#ifndef multi_bandlim_h
#define multi_bandlim_h

#include "Jamstack.h"

class MultiBandlim
{
private:
  SoundMixer4 *    outMixer;
  SoundConnection *cords[4];

  AudioBandlimitedOsc *osc;
  SoundFxEnvelope *    adsr;

public:
  MultiBandlim();
  ~MultiBandlim();
  SoundMixer4 *getOutput();
  void         noteOn(uint8_t pitch);
  void         noteOff();
};

MultiBandlim::MultiBandlim()
{
  outMixer = new SoundMixer4();
  outMixer->gain(0, 1.f);
  outMixer->gain(1, 1.f);

  adsr = new SoundFxEnvelope();
  adsr->attack(25);
  adsr->hold(0);
  adsr->decay(30);
  adsr->sustain(0.9);
  adsr->release(500);

  osc = new AudioBandlimitedOsc();
  osc->waveform(1, 1);
  osc->amplitude(1, 0.5);

  cords[1] = new SoundConnection(*osc, *adsr);
  cords[0] = new SoundConnection(*adsr, *outMixer);
}

MultiBandlim::~MultiBandlim() {}

void MultiBandlim::noteOn(uint8_t pitch)
{
  osc->frequency(1, pitch);
  adsr->noteOn();
}

void MultiBandlim::noteOff()
{
  adsr->noteOff();
}

SoundMixer4 *MultiBandlim::getOutput()
{
  return outMixer;
}

#endif
