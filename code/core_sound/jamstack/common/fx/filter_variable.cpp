#include "filter_variable.h"
#include "utils/dspinst.h"

#define MULT(a, b) (multiply_32x32_rshift32_rounded(a, b) << 2)

void SoundFilterStateVariable::update_fixed(const int16_t *in, int16_t *lp, int16_t *bp, int16_t *hp)
{
  const int16_t *end = in + SOUND_BLOCK_SAMPLES;
  int32_t        input, inputprev;
  int32_t        lowpass, bandpass, highpass;
  int32_t        lowpasstmp, bandpasstmp, highpasstmp;
  int32_t        fmult, damp;

  fmult     = setting_fmult;
  damp      = setting_damp;
  inputprev = state_inputprev;
  lowpass   = state_lowpass;
  bandpass  = state_bandpass;
  do
  {
    input       = (*in++) << 12;
    lowpass     = lowpass + MULT(fmult, bandpass);
    highpass    = ((input + inputprev) >> 1) - lowpass - MULT(damp, bandpass);
    inputprev   = input;
    bandpass    = bandpass + MULT(fmult, highpass);
    lowpasstmp  = lowpass;
    bandpasstmp = bandpass;
    highpasstmp = highpass;
    lowpass     = lowpass + MULT(fmult, bandpass);
    highpass    = input - lowpass - MULT(damp, bandpass);
    bandpass    = bandpass + MULT(fmult, highpass);
    lowpasstmp  = signed_saturate_rshift(lowpass + lowpasstmp, 16, 13);
    bandpasstmp = signed_saturate_rshift(bandpass + bandpasstmp, 16, 13);
    highpasstmp = signed_saturate_rshift(highpass + highpasstmp, 16, 13);
    *lp++       = lowpasstmp;
    *bp++       = bandpasstmp;
    *hp++       = highpasstmp;
  } while(in < end);
  state_inputprev = inputprev;
  state_lowpass   = lowpass;
  state_bandpass  = bandpass;
}

void SoundFilterStateVariable::update_variable(const int16_t *in,
                                               const int16_t *ctl,
                                               int16_t *      lp,
                                               int16_t *      bp,
                                               int16_t *      hp)
{
  const int16_t *end = in + SOUND_BLOCK_SAMPLES;
  int32_t        input, inputprev, control;
  int32_t        lowpass, bandpass, highpass;
  int32_t        lowpasstmp, bandpasstmp, highpasstmp;
  int32_t        fcenter, fmult, damp, octavemult;
  int32_t        n;

  fcenter    = setting_fcenter;
  octavemult = setting_octavemult;
  damp       = setting_damp;
  inputprev  = state_inputprev;
  lowpass    = state_lowpass;
  bandpass   = state_bandpass;
  do
  {
    control = *ctl++;
    control *= octavemult;
    n = control & 0x7FFFFFF;
#ifdef IMPROVE_EXPONENTIAL_ACCURACY

    int32_t x  = n << 3;
    n          = multiply_accumulate_32x32_rshift32_rounded(536870912, x, 1494202713);
    int32_t sq = multiply_32x32_rshift32_rounded(x, x);
    n          = multiply_accumulate_32x32_rshift32_rounded(n, sq, 1934101615);
    n          = n + (multiply_32x32_rshift32_rounded(sq, multiply_32x32_rshift32_rounded(x, 1358044250)) << 1);
    n          = n << 1;
#else

    n = (n + 134217728) << 3;
    n = multiply_32x32_rshift32_rounded(n, n);
    n = multiply_32x32_rshift32_rounded(n, 715827883) << 3;
    n = n + 715827882;
#endif
    n     = n >> (6 - (control >> 27));
    fmult = multiply_32x32_rshift32_rounded(fcenter, n);
    if(fmult > 5378279)
      fmult = 5378279;
    fmult = fmult << 8;

#ifdef IMPROVE_HIGH_FREQUENCY_ACCURACY

    fmult = (multiply_32x32_rshift32_rounded(fmult, 2145892402)
             + multiply_32x32_rshift32_rounded(multiply_32x32_rshift32_rounded(fmult, fmult),
                                               multiply_32x32_rshift32_rounded(fmult, -1383276101)))
            << 1;
#endif

    input       = (*in++) << 12;
    lowpass     = lowpass + MULT(fmult, bandpass);
    highpass    = ((input + inputprev) >> 1) - lowpass - MULT(damp, bandpass);
    inputprev   = input;
    bandpass    = bandpass + MULT(fmult, highpass);
    lowpasstmp  = lowpass;
    bandpasstmp = bandpass;
    highpasstmp = highpass;
    lowpass     = lowpass + MULT(fmult, bandpass);
    highpass    = input - lowpass - MULT(damp, bandpass);
    bandpass    = bandpass + MULT(fmult, highpass);
    lowpasstmp  = signed_saturate_rshift(lowpass + lowpasstmp, 16, 13);
    bandpasstmp = signed_saturate_rshift(bandpass + bandpasstmp, 16, 13);
    highpasstmp = signed_saturate_rshift(highpass + highpasstmp, 16, 13);
    *lp++       = lowpasstmp;
    *bp++       = bandpasstmp;
    *hp++       = highpasstmp;
  } while(in < end);
  state_inputprev = inputprev;
  state_lowpass   = lowpass;
  state_bandpass  = bandpass;
}

void SoundFilterStateVariable::update(void)
{
  sound_block_t *input_block = NULL, *control_block = NULL;
  sound_block_t *lowpass_block = NULL, *bandpass_block = NULL, *highpass_block = NULL;

  input_block   = receiveReadOnly(0);
  control_block = receiveReadOnly(1);
  if(!input_block)
  {
    if(control_block)
      release(control_block);
    return;
  }
  lowpass_block = allocate();
  if(!lowpass_block)
  {
    release(input_block);
    if(control_block)
      release(control_block);
    return;
  }
  bandpass_block = allocate();
  if(!bandpass_block)
  {
    release(input_block);
    release(lowpass_block);
    if(control_block)
      release(control_block);
    return;
  }
  highpass_block = allocate();
  if(!highpass_block)
  {
    release(input_block);
    release(lowpass_block);
    release(bandpass_block);
    if(control_block)
      release(control_block);
    return;
  }

  if(control_block)
  {
    update_variable(
        input_block->data, control_block->data, lowpass_block->data, bandpass_block->data, highpass_block->data);
    release(control_block);
  }
  else
  {
    update_fixed(input_block->data, lowpass_block->data, bandpass_block->data, highpass_block->data);
  }
  release(input_block);
  transmit(lowpass_block, 0);
  release(lowpass_block);
  transmit(bandpass_block, 1);
  release(bandpass_block);
  transmit(highpass_block, 2);
  release(highpass_block);
  return;
}
