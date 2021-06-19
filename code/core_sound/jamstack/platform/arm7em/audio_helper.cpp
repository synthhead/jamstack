#include "audio_helper.h"
#include "memspace.h"

FLASHFUNC void set_audioClock(int nfact, int32_t nmult, uint32_t ndiv, bool force) // sets PLL4
{
  if(!force && (CCM_ANALOG_PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_ENABLE_ZM))
    return;

  CCM_ANALOG_PLL_AUDIO = CCM_ANALOG_PLL_AUDIO_BYPASS_ZM | CCM_ANALOG_PLL_AUDIO_ENABLE_ZM | CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_ZM(2) // 2: 1/4; 1: 1/2; 0: 1/1
                         | CCM_ANALOG_PLL_AUDIO_DIV_SELECT_ZM(nfact);

  CCM_ANALOG_PLL_AUDIO_NUM   = nmult & CCM_ANALOG_PLL_AUDIO_NUM_MASK;
  CCM_ANALOG_PLL_AUDIO_DENOM = ndiv & CCM_ANALOG_PLL_AUDIO_DENOM_MASK;

  CCM_ANALOG_PLL_AUDIO &= ~CCM_ANALOG_PLL_AUDIO_POWERDOWN_ZM;       //Switch on PLL
  while(!(CCM_ANALOG_PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_LOCK_ZM)) {}; //Wait for pll-lock

  const int div_post_pll = 1; // other values: 2,4
  CCM_ANALOG_MISC2 &= ~(CCM_ANALOG_MISC2_DIV_MSB | CCM_ANALOG_MISC2_DIV_LSB);
  if(div_post_pll > 1)
    CCM_ANALOG_MISC2 |= CCM_ANALOG_MISC2_DIV_LSB;
  if(div_post_pll > 3)
    CCM_ANALOG_MISC2 |= CCM_ANALOG_MISC2_DIV_MSB;

  CCM_ANALOG_PLL_AUDIO &= ~CCM_ANALOG_PLL_AUDIO_BYPASS_ZM; //Disable Bypass
}
