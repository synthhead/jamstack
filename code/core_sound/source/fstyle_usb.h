#include "fsl_clock.h"
#include "usb_phy.h"
#include "board.h"

#define CONTROLLER_ID 2U

void USB_DeviceClockInit(void)
{
  usb_phy_config_struct_t phyConfig = {
      BOARD_USB_PHY_D_CAL,
      BOARD_USB_PHY_TXCAL45DP,
      BOARD_USB_PHY_TXCAL45DM,
  };

  CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
  CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);

  USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}
