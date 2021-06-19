#ifndef JAMSTACK_MUL_OSC
#define JAMSTACK_MUL_OSC

#include "mixer.h"
#include "synth_blosc.h"
#include "synth_bandlimosc.h"
#include "synth_sine.h"

namespace Jamstack
{
class Oscillator
{
public:
  Oscillator();
  ~Oscillator();
  SoundAmplifier *output;

private:
  SoundConnection *cordMainOut;
};

Oscillator::Oscillator() {}

Oscillator::~Oscillator() {}

} // namespace Jamstack

#endif /* JAMSTACK_MUL_OSC */
