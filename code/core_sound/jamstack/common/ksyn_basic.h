#include "Jamstack.h"

class SynthBasic
{
private:
  Jamstack::SoundSynthWaveformSine *sine1;
  SoundFxEnvelope *                 adsr;
  SoundMixer4 *                     mixer1;
  SoundConnection *                 cords[4];

public:
  SynthBasic(/* args */);
  ~SynthBasic();
};

SynthBasic::SynthBasic(/* args */) {}

SynthBasic::~SynthBasic() {}
