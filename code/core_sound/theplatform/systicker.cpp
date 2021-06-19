#include "systicker.h"

#include "micro1020.h"
#include "memspace.h"

extern volatile uint32_t systick_cycle_count;

#define SYSTICK_EXT_FREQ 100000

FLASHFUNC void systick_init(void)
{
  SYST_RVR  = (SYSTICK_EXT_FREQ / 1000) - 1;
  SYST_CVR  = 0;
  SYST_CSR  = SYST_CSR_TICKINT | SYST_CSR_ENABLE;
  SCB_SHPR3 = 0x20200000;
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
  systick_cycle_count = ARM_DWT_CYCCNT;
}
