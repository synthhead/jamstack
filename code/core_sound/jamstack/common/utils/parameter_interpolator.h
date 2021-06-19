#ifndef PARAMETER_INTERPOLATOR_H_
#define PARAMETER_INTERPOLATOR_H_

namespace shezlib
{
class ParameterInterpolator
{
public:
  ParameterInterpolator() {}
  ParameterInterpolator(float *state, float new_value, float size) { Init(state, new_value, size); }

  ~ParameterInterpolator() { *state_ = value_; }

  inline void Init(float *state, float new_value, float size)
  {
    state_     = state;
    value_     = *state;
    increment_ = (new_value - *state) / static_cast<float>(size);
  }

  inline float Next()
  {
    value_ += increment_;
    return value_;
  }

  inline float subsample(float t) { return value_ + increment_ * t; }

private:
  float *state_;
  float  value_;
  float  increment_;
};

} // namespace shezlib

#endif
