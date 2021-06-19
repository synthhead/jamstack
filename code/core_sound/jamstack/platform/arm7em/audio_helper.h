#ifndef imxr_hw_h_
#define imxr_hw_h_

#define IMXRT_CACHE_ENABLED 2 // 0=disabled, 1=WT, 2= WB

#include <micro1020.h>

void set_audioClock(int nfact, int32_t nmult, uint32_t ndiv, bool force = false); // sets PLL4

#endif
