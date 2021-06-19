#include "filter_ladder_z_low.h"
#include <math.h>
#include "arm_math.h"
#include <stdint.h>

#define MOOG_PI       ((float)3.14159265358979323846264338327950288)
#define MAX_RESONANCE ((float)1.8)
#define MAX_FREQUENCY ((float)(SOUND_SAMPLING_RATE_EXACT * 0.425f))

float AudioFilterLadderZetaLo::interpolation_coeffs[AudioFilterLadderZetaLo::interpolation_taps]
    = {-14.30851541590154240E-6, 0.001348560352009071,  0.004029285548698377,  0.007644563345368599,
       0.010936856250494802,     0.011982063548666887,  0.008882946305001046,  826.6598116471556070E-6,
       -0.011008071930708746,    -0.023014151355548934, -0.029736402750934567, -0.025405787911977455,
       -0.006012006772274640,    0.028729626071574525,  0.074466890595619062,  0.122757573409695370,
       0.163145421379242955,     0.186152844567746417,  0.186152844567746417,  0.163145421379242955,
       0.122757573409695370,     0.074466890595619062,  0.028729626071574525,  -0.006012006772274640,
       -0.025405787911977455,    -0.029736402750934567, -0.023014151355548934, -0.011008071930708746,
       826.6598116471556070E-6,  0.008882946305001046,  0.011982063548666887,  0.010936856250494802,
       0.007644563345368599,     0.004029285548698377,  0.001348560352009071,  -14.30851541590154240E-6};

#define I_NUM_SAMPLES (SOUND_BLOCK_SAMPLES * INTERPOLATION)

void AudioFilterLadderZetaLo::initpoly()
{
  if(arm_fir_interpolate_init_f32(&interpolation,
                                  INTERPOLATION,
                                  interpolation_taps,
                                  interpolation_coeffs,
                                  interpolation_state,
                                  SOUND_BLOCK_SAMPLES))
  {
    polyCapable = false;
    return;
  }
  if(arm_fir_decimate_init_f32(
         &decimation, interpolation_taps, INTERPOLATION, interpolation_coeffs, decimation_state, I_NUM_SAMPLES))
  {
    polyCapable = false;
    return;
  }

  polyCapable = true;
  polyOn      = true;
}

void AudioFilterLadderZetaLo::interpolationMethod(AudioFilterLadderInterpolation inter_method)
{
  if(inter_method == LADDER_FILTER_INTERPOLATION_FIR_POLY && polyCapable == true)
  {
    polyOn = true;
  }
  else
  {
    polyOn = false;
  }
}

float AudioFilterLadderZetaLo::LPF(float s, int i)
{
  float ft = s * (1.0f / 1.3f) + (0.3f / 1.3f) * z0[i] - z1[i];
  ft       = ft * alpha + z1[i];
  z1[i]    = ft;
  z0[i]    = s;
  return ft;
}

void AudioFilterLadderZetaLo::resonance(float res)
{
  if(res > MAX_RESONANCE)
  {
    res = MAX_RESONANCE;
  }
  else if(res < 0.0f)
  {
    res = 0.0f;
  }
  K = 4.0f * res;
}

void AudioFilterLadderZetaLo::frequency(float c)
{
  Fbase = c;
  compute_coeffs(c);
}

void AudioFilterLadderZetaLo::octaveControl(float octaves)
{
  if(octaves > 7.0f)
  {
    octaves = 7.0f;
  }
  else if(octaves < 0.0f)
  {
    octaves = 0.0f;
  }
  octaveScale = octaves / 32768.0f;
}

void AudioFilterLadderZetaLo::passbandGain(float passbandgain)
{
  pbg = passbandgain;
  if(pbg > 0.5f)
    pbg = 0.5f;
  if(pbg < 0.0f)
    pbg = 0.0f;
  inputDrive(host_overdrive);
}

void AudioFilterLadderZetaLo::inputDrive(float odrv)
{
  host_overdrive = odrv;
  if(host_overdrive > 1.0f)
  {
    if(host_overdrive > 4.0f)
      host_overdrive = 4.0f;

    overdrive = 1.0f + (host_overdrive - 1.0f) * (1.0f - pbg);
  }
  else
  {
    overdrive = host_overdrive;
    if(overdrive < 0.0f)
      overdrive = 0.0f;
  }
}

void AudioFilterLadderZetaLo::compute_coeffs(float c)
{
  if(c > MAX_FREQUENCY)
  {
    c = MAX_FREQUENCY;
  }
  else if(c < 5.0f)
  {
    c = 5.0f;
  }
  float wc  = c * (float)(2.0f * MOOG_PI / ((float)INTERPOLATION * SOUND_SAMPLING_RATE_EXACT));
  float wc2 = wc * wc;
  alpha     = 0.9892f * wc - 0.4324f * wc2 + 0.1381f * wc * wc2 - 0.0202f * wc2 * wc2;

  Qadjust = 1.006f + 0.0536f * wc - 0.095f * wc2 - 0.05f * wc2 * wc2;
}

bool AudioFilterLadderZetaLo::resonating()
{
  for(int i = 0; i < 4; i++)
  {
    if(fabsf(z0[i]) > 0.0001f)
      return true;
    if(fabsf(z1[i]) > 0.0001f)
      return true;
  }
  return false;
}

static inline float fast_exp2f(float x)
{
  float i;
  float f = modff(x, &i);
  f *= 0.693147f / 256.0f;
  f += 1.0f;
  f *= f;
  f *= f;
  f *= f;
  f *= f;
  f *= f;
  f *= f;
  f *= f;
  f *= f;
  f = ldexpf(f, i);
  return f;
}

static inline float fast_tanh(float x)
{
  if(x > 3.0f)
    return 1.0f;
  if(x < -3.0f)
    return -1.0f;
  float x2 = x * x;
  return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

void AudioFilterLadderZetaLo::update(void)
{
  sound_block_t *blocka, *blockb, *blockc;
  float          Ktot        = K;
  bool           FCmodActive = true;
  bool           QmodActive  = true;

  blocka = receiveWritable(0);
  blockb = receiveReadOnly(1);
  blockc = receiveReadOnly(2);
  if(!blocka)
  {
    if(resonating())
    {
      blocka = allocate();
    }
    if(!blocka)
    {
      if(blockb)
        release(blockb);
      if(blockc)
        release(blockc);
      return;
    }
    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      blocka->data[i] = 0;
    }
  }
  if(!blockb)
  {
    FCmodActive = false;
  }
  if(!blockc)
  {
    QmodActive = false;
  }
  if(polyOn == true)
  {
    float blockOS[I_NUM_SAMPLES], blockIn[SOUND_BLOCK_SAMPLES];
    float blockOutOS[I_NUM_SAMPLES], blockOut[SOUND_BLOCK_SAMPLES];
    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      blockIn[i] = blocka->data[i] * overdrive * (float)INTERPOLATION / 32768.0f;
    }
    arm_fir_interpolate_f32(&interpolation, blockIn, blockOS, SOUND_BLOCK_SAMPLES);

    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      if(FCmodActive)
      {
        float FCmod = blockb->data[i] * octaveScale;
        float ftot  = Fbase * fast_exp2f(FCmod);
        if(ftot > MAX_FREQUENCY)
          ftot = MAX_FREQUENCY;
        compute_coeffs(ftot);
      }
      if(QmodActive)
      {
        float Qmod = blockc->data[i] * (1.0f / 32768.0f);
        Ktot       = K + 4.0f * Qmod;
      }
      if(Ktot > MAX_RESONANCE * 4.0f)
      {
        Ktot = MAX_RESONANCE * 4.0f;
      }
      else if(Ktot < 0.0f)
      {
        Ktot = 0.0f;
      }
      for(int os = 0; os < INTERPOLATION; os++)
      {
        float input            = blockOS[i * 4 + os];
        float u                = input - (z1[3] - pbg * input) * Ktot * Qadjust;
        u                      = fast_tanh(u);
        float stage1           = LPF(u, 0);
        float stage2           = LPF(stage1, 1);
        float stage3           = LPF(stage2, 2);
        float stage4           = LPF(stage3, 3);
        blockOutOS[i * 4 + os] = stage4;
      }
    }
    arm_fir_decimate_f32(&decimation, blockOutOS, blockOut, I_NUM_SAMPLES);
    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      blocka->data[i] = (float)(blockOut[i]) * 32768.0f;
    }
  }
  else
  {
    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      float input = blocka->data[i] * overdrive * (1.0f / 32768.0f);
      if(FCmodActive)
      {
        float FCmod = blockb->data[i] * octaveScale;
        float ftot  = Fbase * fast_exp2f(FCmod);
        if(ftot > MAX_FREQUENCY)
          ftot = MAX_FREQUENCY;
        compute_coeffs(ftot);
      }
      if(QmodActive)
      {
        float Qmod = blockc->data[i] * (1.0f / 32768.0f);
        Ktot       = K + 4.0f * Qmod;
      }
      if(Ktot > MAX_RESONANCE * 4.0f)
      {
        Ktot = MAX_RESONANCE * 4.0f;
      }
      else if(Ktot < 0.0f)
      {
        Ktot = 0.0f;
      }
      float total  = 0.0f;
      float interp = 0.0f;
      for(int os = 0; os < INTERPOLATION; os++)
      {
        float u      = (interp * oldinput + (1.0f - interp) * input) - (z1[3] - pbg * input) * Ktot * Qadjust;
        u            = fast_tanh(u);
        float stage1 = LPF(u, 0);
        float stage2 = LPF(stage1, 1);
        float stage3 = LPF(stage2, 2);
        float stage4 = LPF(stage3, 3);
        total += stage4 * (1.0f / (float)INTERPOLATION);
        interp += (1.0f / (float)INTERPOLATION);
      }
      oldinput        = input;
      blocka->data[i] = total * 32768.0f;
    }
  }
  transmit(blocka);
  release(blocka);

  if(blockb)
    release(blockb);
  if(blockc)
    release(blockc);
}
