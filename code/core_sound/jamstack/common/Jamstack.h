#ifndef JAMSTACK_H_
#define JAMSTACK_H_

#if defined(__ARM_ARCH_7EM__)

#define IrqAudioDisable() (__NVIC_DisableIRQ(Reserved87_IRQn))
#define IrqAudioEnable()  (__NVIC_EnableIRQ(Reserved87_IRQn))
#include "../platform/arm7em/input_i2s.h"
#include "../platform/arm7em/output_i2s.h"

#else

#define IrqAudioDisable()
#define IrqAudioEnable()
#include "../platform/sim/output_i2s.h"

#endif

#include "mixer.h"
#include "clocker.h"
#include "tempo.h"
#include "note_stack.h"
#include "voice_manager.h"
#include "voice_allocator.h"

#include "fx/effect_envelope.h"
#include "fx/effect_fade.h"
#include "fx/effect_delay.h"
#include "fx/effect_granular.h"
#include "fx/filter_ladder_a.h"
#include "fx/filter_ladder_b.h"
#include "fx/filter_ladder_c.h"
#include "fx/filter_variable.h"
#include "fx/filter_diode_ladder.h"

#include "synth/synth_karplusstrong.h"
#include "synth/synth_sine.h"
#include "synth/synth_tonewheel.h"
#include "synth/synth_variable_saw.h"
#include "synth/synth_noisewhite.h"
#include "synth/synth_noisepink.h"
#include "synth/synth_bandlimosc.h"
#include "synth/synth_dc.h"
#include "synth/synth_blosc.h"

#include "play_memory.h"
#include "com_basicsynth.h"

#endif
