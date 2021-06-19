#ifndef dspinst_h_
#define dspinst_h_

#include <stdint.h>

static inline int32_t signed_saturate_rshift(int32_t val, int bits, int rshift) __attribute__((always_inline, unused));
static inline int32_t signed_saturate_rshift(int32_t val, int bits, int rshift)
{
#if defined(__ARM_ARCH_7EM__)

  int32_t out;
  asm volatile("ssat %0, %1, %2, asr %3" : "=r"(out) : "I"(bits), "r"(val), "I"(rshift));
  return out;

#else

  int32_t out, max;
  out = val >> rshift;
  max = 1 << (bits - 1);
  if(out >= 0)
  {
    if(out > max - 1)
      out = max - 1;
  }
  else
  {
    if(out < -max)
      out = -max;
  }
  return out;

#endif
}

static inline int16_t saturate16(int32_t val) __attribute__((always_inline, unused));
static inline int16_t saturate16(int32_t val)
{
#if defined(__ARM_ARCH_7EM__)
  int16_t out;
  int32_t tmp;
  asm volatile("ssat %0, %1, %2" : "=r"(tmp) : "I"(16), "r"(val));
  out = (int16_t)(tmp & 0xffff);
  return out;
#else
  return 0;
#endif
}

static inline int32_t signed_multiply_32x16b(int32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t signed_multiply_32x16b(int32_t a, uint32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("smulwb %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return ((int64_t)a * (int16_t)(b & 0xFFFF)) >> 16;
#endif
}

static inline int32_t signed_multiply_32x16t(int32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t signed_multiply_32x16t(int32_t a, uint32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("smulwt %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return ((int64_t)a * (int16_t)(b >> 16)) >> 16;
#endif
}

static inline int32_t multiply_32x32_rshift32(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_32x32_rshift32(int32_t a, int32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("smmul %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return ((int64_t)a * (int64_t)b) >> 32;
#endif
}

static inline int32_t multiply_32x32_rshift32_rounded(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_32x32_rshift32_rounded(int32_t a, int32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("smmulr %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return (((int64_t)a * (int64_t)b) + 0x8000000) >> 32;
#endif
}

static inline int32_t multiply_accumulate_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b)
    __attribute__((always_inline, unused));
static inline int32_t multiply_accumulate_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("smmlar %0, %2, %3, %1" : "=r"(out) : "r"(sum), "r"(a), "r"(b));
  return out;
#else
  return sum + ((((int64_t)a * (int64_t)b) + 0x8000000) >> 32);
#endif
}

static inline int32_t multiply_subtract_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b)
    __attribute__((always_inline, unused));
static inline int32_t multiply_subtract_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("smmlsr %0, %2, %3, %1" : "=r"(out) : "r"(sum), "r"(a), "r"(b));
  return out;
#else
  return sum - ((((int64_t)a * (int64_t)b) + 0x8000000) >> 32);
#endif
}

static inline uint32_t pack_16t_16t(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline uint32_t pack_16t_16t(int32_t a, int32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("pkhtb %0, %1, %2, asr #16" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return (a & 0xFFFF0000) | ((uint32_t)b >> 16);
#endif
}

static inline uint32_t pack_16t_16b(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline uint32_t pack_16t_16b(int32_t a, int32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("pkhtb %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return (a & 0xFFFF0000) | (b & 0x0000FFFF);
#endif
}

static inline uint32_t pack_16b_16b(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline uint32_t pack_16b_16b(int32_t a, int32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("pkhbt %0, %1, %2, lsl #16" : "=r"(out) : "r"(b), "r"(a));
  return out;
#else
  return (a << 16) | (b & 0x0000FFFF);
#endif
}

static inline uint32_t signed_add_16_and_16(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline uint32_t signed_add_16_and_16(uint32_t a, uint32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("qadd16 %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return (((a & 0xFFFF0000) + (b & 0xFFFF0000)) << 16) | ((a & 0x0000FFFF) + (b & 0x0000FFFF));
#endif
}

static inline int32_t signed_subtract_16_and_16(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t signed_subtract_16_and_16(int32_t a, int32_t b)
{
  int32_t out;
  asm volatile("qsub16 %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int32_t signed_halving_add_16_and_16(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t signed_halving_add_16_and_16(int32_t a, int32_t b)
{
  int32_t out;
  asm volatile("shadd16 %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int32_t signed_halving_subtract_16_and_16(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t signed_halving_subtract_16_and_16(int32_t a, int32_t b)
{
  int32_t out;
  asm volatile("shsub16 %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int32_t signed_multiply_accumulate_32x16b(int32_t sum, int32_t a, uint32_t b)
    __attribute__((always_inline, unused));
static inline int32_t signed_multiply_accumulate_32x16b(int32_t sum, int32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("smlawb %0, %2, %3, %1" : "=r"(out) : "r"(sum), "r"(a), "r"(b));
  return out;
}

static inline int32_t signed_multiply_accumulate_32x16t(int32_t sum, int32_t a, uint32_t b)
    __attribute__((always_inline, unused));
static inline int32_t signed_multiply_accumulate_32x16t(int32_t sum, int32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("smlawt %0, %2, %3, %1" : "=r"(out) : "r"(sum), "r"(a), "r"(b));
  return out;
}

static inline uint32_t logical_and(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline uint32_t logical_and(uint32_t a, uint32_t b)
{
  asm volatile("and %0, %1" : "+r"(a) : "r"(b));
  return a;
}

static inline int32_t multiply_16tx16t_add_16bx16b(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_16tx16t_add_16bx16b(uint32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("smuad %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int32_t multiply_16tx16b_add_16bx16t(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_16tx16b_add_16bx16t(uint32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("smuadx %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int64_t multiply_accumulate_16tx16t_add_16bx16b(int64_t sum, uint32_t a, uint32_t b)
{
  asm volatile("smlald %Q0, %R0, %1, %2" : "+r"(sum) : "r"(a), "r"(b));
  return sum;
}

static inline int64_t multiply_accumulate_16tx16b_add_16bx16t(int64_t sum, uint32_t a, uint32_t b)
{
  asm volatile("smlaldx %Q0, %R0, %1, %2" : "+r"(sum) : "r"(a), "r"(b));
  return sum;
}

static inline int32_t multiply_16bx16b(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_16bx16b(uint32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("smulbb %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int32_t multiply_16bx16t(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_16bx16t(uint32_t a, uint32_t b)
{
#if defined(__ARM_ARCH_7EM__)
  int32_t out;
  asm volatile("smulbt %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
#else
  return (a * (b & 0xFFFF0000));
#endif
}

static inline int32_t multiply_16tx16b(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_16tx16b(uint32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("smultb %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int32_t multiply_16tx16t(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_16tx16t(uint32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("smultt %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline int32_t substract_32_saturate(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t substract_32_saturate(uint32_t a, uint32_t b)
{
  int32_t out;
  asm volatile("qsub %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));
  return out;
}

static inline uint32_t get_q_psr(void) __attribute__((always_inline, unused));
static inline uint32_t get_q_psr(void)
{
  uint32_t out;
  asm("mrs %0, APSR" : "=r"(out));
  return (out & 0x8000000) >> 27;
}

static inline void clr_q_psr(void) __attribute__((always_inline, unused));
static inline void clr_q_psr(void)
{
  uint32_t t;
  asm("mov %[t],#0\n"
      "msr APSR_nzcvq,%0\n"
      : [t] "=&r"(t)::"cc");
}

static inline int32_t FRACMUL_SHL(int32_t x, int32_t y, int z)
{
  int32_t t, t2;
  asm("smull    %[t], %[t2], %[a], %[b]\n\t"
      "mov      %[t2], %[t2], asl %[c]\n\t"
      "orr      %[t], %[t2], %[t], lsr %[d]\n\t"
      : [t] "=&r"(t), [t2] "=&r"(t2)
      : [a] "r"(x), [b] "r"(y), [c] "Mr"((z) + 1), [d] "Mr"(31 - (z)));
  return t;
}

#endif
