#ifndef DSP_POLYBLEP_H_
#define DSP_POLYBLEP_H_

namespace shezlib
{
inline float ThisBlepSample(float t)
{
  return 0.5f * t * t;
}

inline float NextBlepSample(float t)
{
  t = 1.0f - t;
  return -0.5f * t * t;
}

inline float NextIntegratedBlepSample(float t)
{
  const float t1 = 0.5f * t;
  const float t2 = t1 * t1;
  const float t4 = t2 * t2;
  return 0.1875f - t1 + 1.5f * t2 - t4;
}

inline float ThisIntegratedBlepSample(float t)
{
  return NextIntegratedBlepSample(1.0f - t);
}

} // namespace shezlib

#endif // DSP_POLYBLEP_H_
