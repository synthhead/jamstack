#include "com_basicsynth.h"
#include <arm_math.h>

using namespace Jamstack;

BasicSynth::BasicSynth()
{
  // sine1 = nullptr;
  // saw1  = nullptr;
  // blosc = nullptr;

  // cordMainOut = nullptr;

  output = new SoundMixer4();
  output->gain(0, 0.6f);
  output->gain(1, 0.6f);

  filterLadderD = new SoundFilterLadderD();

  adsr1 = new SoundFxEnvelope();
  adsr1->attack(2);
  adsr1->hold(0);
  adsr1->decay(30);
  adsr1->sustain(0.9);
  adsr1->release(500);

  // sine1 = new SoundSynthWaveformSine();
  // sine1->frequency(440.);
  // sine1->amplitude(0.5);

  saw1 = new SoundSynthVariableSaw();
  saw1->setMidiNote(69U);
  saw1->setAmplitude(0.5f);

  // blosc = new SoundSynthBlosc();
  // blosc->setWaveform(1);
  // blosc->setFreq(440.f);
  // blosc->setAmp(0.5f);

  patcher1 = new Jamstack::Patcher();

  cords = new SoundConnection[4]{
      SoundConnection(*saw1, *adsr1),
      SoundConnection(*adsr1, *filterLadderD),
      SoundConnection(*filterLadderD, *output),
      SoundConnection(*patcher1->output(), 0, *output, 1),
  };

  // cordMainOut = new SoundConnection(*adsr1, 0, *output, 1);
}

BasicSynth::~BasicSynth() {}

void BasicSynth::disableFilter()
{
  cordOscToFilter->disconnect();
}

void BasicSynth::onNoteOn(int note, int velocity)
{
  //  float freq = powf(2.0, note * (1.0 / 12.0) + 3.0313597); //? Fix warnings

  // sine1->frequency(freq);
  // blosc->setFreq(freq);
  saw1->setMidiNote(note);
  adsr1->noteOn();
  // adsr2->noteOn();
  // filterEnv->noteOn();
}

void BasicSynth::onNoteOff(int note)
{
  adsr1->noteOff();
  // adsr2->noteOff();
  // filterEnv->noteOff();
}

void BasicSynth::onControlChange(int id, int value)
{
  if(id == 0)
  {
    float val = (float)value / 127.f;
    filterLadderD->setCutoff(val);
  }
  else if(id == 1)
  {
    float val = (float)value / 64.f;
    filterLadderD->setResonance(val);
  }
  else if(id == 124)
  {
    // patcher1->testConnect();
  }
  else if(id == 125)
  {
    // patcher1->testDisconnect();
  }
  else if(id == 126)
  {
    changeEngine((int8_t)value);
  }
}

void BasicSynth::changeEngine(int8_t engine)
{
  static int8_t lastEngine = -1;

  if(engine != lastEngine)
  {
    // disconnect
    if(cordMainOut != nullptr)
    {
      delete cordMainOut;
      cordMainOut = nullptr; // ?
    }

    // destroy old engine, create a new one and connect
    switch(lastEngine)
    {
      case(int8_t)Engines::BASIC:
        delete sine1;
        sine1 = nullptr;
        break;

      case(int8_t)Engines::VAR_SAW:
        delete saw1;
        saw1 = nullptr;
        break;

      case(int8_t)Engines::BLOSC:
        delete blosc;
        blosc = nullptr;
        break;

      default: break;
    }

    switch(engine)
    {
      case(int8_t)Engines::BASIC:
        sine1 = new SoundSynthWaveformSine();
        sine1->frequency(330.f);
        sine1->amplitude(0.5);
        cordMainOut = new SoundConnection(*sine1, *output);
        break;

      case(int8_t)Engines::VAR_SAW:
        saw1 = new SoundSynthVariableSaw();
        saw1->setMidiNote(69U); // 440
        saw1->setAmplitude(0.5f);
        cordMainOut = new SoundConnection(*saw1, *output);
        break;

      case(int8_t)Engines::BLOSC:
        blosc = new SoundSynthBlosc();
        blosc->setWaveform(1);
        blosc->setFreq(550.f);
        blosc->setAmp(0.5f);
        cordMainOut = new SoundConnection(*blosc, *output);
        break;

      default: break;
    }

    lastEngine = engine;
  }
}
