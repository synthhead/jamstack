#ifndef synth_variable_saw_h_
#define synth_variable_saw_h_

#include "SoundStream.h"

#include "utils/parameter_interpolator.h"
#include "utils/polyblep.h"
#include "utils/units.h"

#define CONSTRAIN(var, min, max) \
  if(var < (min))                \
  {                              \
    var = (min);                 \
  }                              \
  else if(var > (max))           \
  {                              \
    var = (max);                 \
  }

const float kMaxFrequency          = 0.25f;
const float kVariableSawNotchDepth = 0.2f;
const float a0                     = (440.0f / 8.0f) / SOUND_SAMPLING_RATE_EXACT;

inline float NoteToFrequency(float midi_note)
{
  midi_note -= 9.0f;
  CONSTRAIN(midi_note, -128.0f, 127.0f);
  return a0 * 0.25f * shezlib::SemitonesToRatio(midi_note);
}

class SoundSynthVariableSaw : public SoundStream
{
public:
  SoundSynthVariableSaw(void) : SoundStream(1, inputQueueArray)
  {
    phase_       = 0.0f;
    next_sample_ = 0.0f;
    previous_pw_ = 0.5f;
    high_        = false;

    frequency_ = 0.01f;
    pw_        = 0.5f;
    waveshape_ = 0.0f;

    frequency = 0.01f;
    pw        = 0.5f;
    waveshape = 0.0f;
  }

  void setFrequency(float freq) { frequency = freq; }

  void setMidiNote(uint8_t pitch) { frequency = NoteToFrequency(pitch); }

  void    setPhase(float angle) {}
  void    setAmplitude(float n) {}
  void    setOffset(float n) {}
  void    setPulseWidth(float n) {}
  void    begin(short t_type) {}
  void    begin(float t_amp, float t_freq, short t_type) {}
  void    arbitraryWaveform(const int16_t *data, float maxFreq) {}
  int16_t to_int16(float f)
  {
    if(f > 1.0)
      f = 1.0;
    if(f < -1.0)
      f = -1.0;
    return (int16_t)(f * 0x7fff);
  }

  virtual void update(void) override
  {
    sound_block_t *block;
    int16_t *      bp;

    block = allocate();
    if(!block)
      return;

    bp = block->data;

    if(frequency >= kMaxFrequency)
    {
      frequency = kMaxFrequency;
    }

    if(frequency >= 0.25f)
    {
      pw = 0.5f;
    }
    else
    {
      CONSTRAIN(pw, frequency * 2.0f, 1.0f - 2.0f * frequency);
    }

    shezlib::ParameterInterpolator fm(&frequency_, frequency, SOUND_BLOCK_SAMPLES);
    shezlib::ParameterInterpolator pwm(&pw_, pw, SOUND_BLOCK_SAMPLES);
    shezlib::ParameterInterpolator waveshape_modulation(&waveshape_, waveshape, SOUND_BLOCK_SAMPLES);

    float next_sample = next_sample_;

    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      float this_sample = next_sample;
      next_sample       = 0.0f;

      const float frequency       = fm.Next();
      const float pw              = pwm.Next();
      const float waveshape       = waveshape_modulation.Next();
      const float triangle_amount = waveshape;
      const float notch_amount    = 1.0f - waveshape;
      const float slope_up        = 1.0f / (pw);
      const float slope_down      = 1.0f / (1.0f - pw);

      phase_ += frequency;

      if(!high_ && phase_ >= pw)
      {
        const float triangle_step = (slope_up + slope_down) * frequency * triangle_amount;
        const float notch         = (kVariableSawNotchDepth + 1.0f - pw) * notch_amount;
        const float t             = (phase_ - pw) / (previous_pw_ - pw + frequency);
        this_sample += notch * shezlib::ThisBlepSample(t);
        next_sample += notch * shezlib::NextBlepSample(t);
        this_sample -= triangle_step * shezlib::ThisIntegratedBlepSample(t);
        next_sample -= triangle_step * shezlib::NextIntegratedBlepSample(t);
        high_ = true;
      }
      else if(phase_ >= 1.0f)
      {
        phase_ -= 1.0f;
        const float triangle_step = (slope_up + slope_down) * frequency * triangle_amount;
        const float notch         = (kVariableSawNotchDepth + 1.0f) * notch_amount;
        const float t             = phase_ / frequency;
        this_sample -= notch * shezlib::ThisBlepSample(t);
        next_sample -= notch * shezlib::NextBlepSample(t);
        this_sample += triangle_step * shezlib::ThisIntegratedBlepSample(t);
        next_sample += triangle_step * shezlib::NextIntegratedBlepSample(t);
        high_ = false;
      }

      next_sample += ComputeNaiveSample(phase_, pw, slope_up, slope_down, triangle_amount, notch_amount);
      previous_pw_ = pw;

      bp[i] = to_int16((2.0f * this_sample - 1.0f) / (1.0f + kVariableSawNotchDepth));
    }

    next_sample_ = next_sample;

    transmit(block);
    release(block);

    return;
  }

private:
  inline float ComputeNaiveSample(float phase,
                                  float pw,
                                  float slope_up,
                                  float slope_down,
                                  float triangle_amount,
                                  float notch_amount) const
  {
    float notch_saw = phase < pw ? phase : 1.0f + kVariableSawNotchDepth;
    float triangle  = phase < pw ? phase * slope_up : 1.0f - (phase - pw) * slope_down;
    return notch_saw * notch_amount + triangle * triangle_amount;
  }

  sound_block_t *inputQueueArray[1];

  float frequency;
  float pw;
  float waveshape;

  float phase_;
  float next_sample_;
  float previous_pw_;
  bool  high_;

  float frequency_;
  float pw_;
  float waveshape_;
};

#endif
