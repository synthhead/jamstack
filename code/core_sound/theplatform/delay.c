#include "core.h"
#include "arm_math.h"

volatile uint32_t systick_millis_count    = 0;
volatile uint32_t systick_cycle_count     = 0;
volatile uint32_t cpu_cyc_to_microseconds = 0xFFFFFFU / (uint32_t)(500000000U / 1000000U);
uint32_t          systick_safe_read; // micros() sync

#define SYSTICK_EXT_FREQ 100000

uint32_t micros(void)
{
  uint32_t smc, scc;

  do
  {
    __LDREXW(&systick_safe_read);
    smc = systick_millis_count;
    scc = systick_cycle_count;
  } while(__STREXW(1, &systick_safe_read));

  uint32_t cycle_count = ARM_DWT_CYCCNT;
  asm volatile("" : : : "memory");
  uint32_t cycle_count_delta = cycle_count - scc;

  uint32_t fraction = ((uint64_t)cycle_count_delta * cpu_cyc_to_microseconds) >> 32;
  if(fraction > 1000)
    fraction = 1000;

  uint32_t usec = 1000 * smc + fraction;
  return usec;
}
