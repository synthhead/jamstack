// #ifndef CORE_H_
// #define CORE_H_

#pragma once

#include "micro1020.h"

#ifdef __cplusplus
extern "C"
{
#endif

  extern volatile uint32_t cpu_cyc_to_microseconds;
  extern volatile uint32_t systick_millis_count;

  static inline uint32_t millis(void) __attribute__((always_inline, unused));
  static inline uint32_t millis(void) { return systick_millis_count; }

  uint32_t micros(void);

  static inline void delayMicroseconds(uint32_t) __attribute__((always_inline, unused));
  static inline void delayMicroseconds(uint32_t usec)
  {
    uint32_t begin  = ARM_DWT_CYCCNT;
    uint32_t cycles = F_CPU_ACTUAL / 1000000 * usec;
    while(ARM_DWT_CYCCNT - begin < cycles) {}
  }

#define THE_PLATFORM_DELNANOSEC_DIV_NUM (1000000000UL >> 16)
  static inline void delayNanoseconds(uint32_t) __attribute__((always_inline, unused));
  static inline void delayNanoseconds(uint32_t nsec)
  {
    uint32_t begin  = ARM_DWT_CYCCNT;
    uint32_t cycles = ((F_CPU_ACTUAL >> 16) * nsec) / THE_PLATFORM_DELNANOSEC_DIV_NUM;
    while(ARM_DWT_CYCCNT - begin < cycles) {}
  }
#undef THE_PLATFORM_DELNANOSEC_DIV_NUM

#ifdef __cplusplus
}
#endif

// #endif
