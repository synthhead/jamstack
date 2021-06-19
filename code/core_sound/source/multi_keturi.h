#ifndef multi_keturi_h
#define multi_keturi_h

#include "Jamstack.h"

class MultiKeturi
{
private:
  SoundMixer4 *    outMixer;
  SoundConnection *cords[4];

  Jamstack::SoundSynthWaveformSine *sine1;
  SoundSynthVariableSaw *           variableSaw;
  SoundSynthNoisePink *             pinkNoise;
  SoundFxEnvelope *                 adsr;
  SoundMixer4 *                     mixer1;

public:
  MultiKeturi();
  ~MultiKeturi();
  SoundMixer4 *getOutput();
  void         noteOn(uint8_t pitch);
  void         noteOff();
};

MultiKeturi::MultiKeturi()
{
  adsr = new SoundFxEnvelope();
  adsr->attack(25);
  adsr->hold(0);
  adsr->decay(30);
  adsr->sustain(0.9);
  adsr->release(500);

  mixer1 = new SoundMixer4();
  mixer1->gain(0, 1.f);
  mixer1->gain(1, 1.f);

  outMixer = new SoundMixer4();
  outMixer->gain(0, 1.f);
  outMixer->gain(1, 1.f);

  variableSaw = new SoundSynthVariableSaw();
  variableSaw->setMidiNote(63U);
  variableSaw->setAmplitude(0.5f);

  pinkNoise = new SoundSynthNoisePink();
  pinkNoise->amplitude(0.2f);

  cords[3] = new SoundConnection(*variableSaw, *mixer1);
  cords[2] = new SoundConnection(*pinkNoise, 0, *mixer1, 1);
  cords[1] = new SoundConnection(*mixer1, *adsr);
  cords[0] = new SoundConnection(*adsr, *outMixer);
}

MultiKeturi::~MultiKeturi() {}

void MultiKeturi::noteOn(uint8_t pitch)
{
  variableSaw->setMidiNote(pitch);
  adsr->noteOn();
}

void MultiKeturi::noteOff()
{
  adsr->noteOff();
}

SoundMixer4 *MultiKeturi::getOutput()
{
  return outMixer;
}

#endif
