#ifndef filter_ladder_h_
#define filter_ladder_h_

#include <SoundStream.h>
#include "jamstackcommons.h"
#include "arm_math.h"

enum AudioFilterLadderInterpolation
{
  LADDER_FILTER_INTERPOLATION_LINEAR,
  LADDER_FILTER_INTERPOLATION_FIR_POLY
};

class AudioFilterLadderZetaLo : public SoundStream
{
public:
  AudioFilterLadderZetaLo() : SoundStream(3, inputQueueArray) { initpoly(); };
  void frequency(float FC);
  void resonance(float reson);
  void octaveControl(float octaves);
  void passbandGain(float passbandgain);
  void inputDrive(float drv);
  void interpolationMethod(AudioFilterLadderInterpolation inter_method);

  virtual void update(void);

private:
  static const int INTERPOLATION      = 4;
  static const int interpolation_taps = 36;

  float interpolation_state[(SOUND_BLOCK_SAMPLES - 1) + interpolation_taps / INTERPOLATION];
  float decimation_state[(SOUND_BLOCK_SAMPLES * INTERPOLATION - 1) + interpolation_taps];

  arm_fir_interpolate_instance_f32 interpolation;
  arm_fir_decimate_instance_f32    decimation;

  static float interpolation_coeffs[interpolation_taps];

  float LPF(float s, int i);
  void  compute_coeffs(float fc);
  void  initpoly();
  bool  resonating();
  bool  polyCapable    = false;
  bool  polyOn         = false;
  float alpha          = 1.0;
  float beta[4]        = {0.0, 0.0, 0.0, 0.0};
  float z0[4]          = {0.0, 0.0, 0.0, 0.0};
  float z1[4]          = {0.0, 0.0, 0.0, 0.0};
  float K              = 1.0f;
  float Fbase          = 1000;
  float Qadjust        = 1.0f;
  float octaveScale    = 1.0f / 32768.0f;
  float pbg            = 0.5f;
  float overdrive      = 0.5f;
  float host_overdrive = 1.0f;
  float oldinput       = 0;

  sound_block_t *inputQueueArray[3];
};

#endif
