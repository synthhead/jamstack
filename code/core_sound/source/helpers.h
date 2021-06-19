#ifndef helpers_custom_h
#define helpers_custom_h

FLASHFUNC void usb_pll_start()
{
  while(1)
  {
    uint32_t n = CCM_ANALOG_PLL_USB1; // pg 759
    // PRINTF("CCM_ANALOG_PLL_USB1=%08lX\n", n);
    if(n & CCM_ANALOG_PLL_USB1_DIV_SELECT_ZM)
    {
      // PRINTF("  ERROR, 528 MHz mode!\n");                           // never supposed to use this mode!
      CCM_ANALOG_PLL_USB1_CLR = 0xC000;                             // bypass 24 MHz
      CCM_ANALOG_PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_ZM;      // bypass
      CCM_ANALOG_PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_POWER_ZM |      // power down
                                CCM_ANALOG_PLL_USB1_DIV_SELECT_ZM | // use 480 MHz
                                CCM_ANALOG_PLL_USB1_ENABLE_ZM |     // disable
                                CCM_ANALOG_PLL_USB1_EN_USB_CLKS_ZM; // disable usb
      continue;
    }
    if(!(n & CCM_ANALOG_PLL_USB1_ENABLE_ZM))
    {
      // PRINTF("  enable PLL\n");
      // TODO: should this be done so early, or later??
      CCM_ANALOG_PLL_USB1_SET = CCM_ANALOG_PLL_USB1_ENABLE_ZM;
      continue;
    }
    if(!(n & CCM_ANALOG_PLL_USB1_POWER_ZM))
    {
      // PRINTF("  power up PLL\n");
      CCM_ANALOG_PLL_USB1_SET = CCM_ANALOG_PLL_USB1_POWER_ZM;
      continue;
    }
    if(!(n & CCM_ANALOG_PLL_USB1_LOCK_ZM))
    {
      // PRINTF("  wait for lock\n");
      continue;
    }
    if(n & CCM_ANALOG_PLL_USB1_BYPASS_ZM)
    {
      // PRINTF("  turn off bypass\n");
      CCM_ANALOG_PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_BYPASS_ZM;
      continue;
    }
    if(!(n & CCM_ANALOG_PLL_USB1_EN_USB_CLKS_ZM))
    {
      // PRINTF("  enable USB clocks\n");
      CCM_ANALOG_PLL_USB1_SET = CCM_ANALOG_PLL_USB1_EN_USB_CLKS_ZM;
      continue;
    }
    return; // everything OK
  }
}

#endif
