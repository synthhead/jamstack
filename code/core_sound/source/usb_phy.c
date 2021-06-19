/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// #include "usb.h"
#include "fsl_device_registers.h"
#include "MIMXRT1021.h"
#include "usb_phy.h"

#include <stdint.h>
#include <stdio.h>

// FROM USB.C ----------------------------
/*
 * Component ID used by tools
 *
 * FSL_COMPONENT_ID "middleware.usb.stack_common"
 */

/*! @brief USB error code */
typedef enum _usb_status
{
  kStatus_USB_Success = 0x00U, /*!< Success */
  kStatus_USB_Error,           /*!< Failed */

  kStatus_USB_Busy,                       /*!< Busy */
  kStatus_USB_InvalidHandle,              /*!< Invalid handle */
  kStatus_USB_InvalidParameter,           /*!< Invalid parameter */
  kStatus_USB_InvalidRequest,             /*!< Invalid request */
  kStatus_USB_ControllerNotFound,         /*!< Controller cannot be found */
  kStatus_USB_InvalidControllerInterface, /*!< Invalid controller interface */

  kStatus_USB_NotSupported,   /*!< Configuration is not supported */
  kStatus_USB_Retry,          /*!< Enumeration get configuration retry */
  kStatus_USB_TransferStall,  /*!< Transfer stalled */
  kStatus_USB_TransferFailed, /*!< Transfer failed */
  kStatus_USB_AllocFail,      /*!< Allocation failed */
  kStatus_USB_LackSwapBuffer, /*!< Insufficient swap buffer for KHCI */
  kStatus_USB_TransferCancel, /*!< The transfer cancelled */
  kStatus_USB_BandwidthFail,  /*!< Allocate bandwidth failed */
  kStatus_USB_MSDStatusFail,  /*!< For MSD, the CSW status means fail */
  kStatus_USB_EHCIAttached,
  kStatus_USB_EHCIDetached,
  kStatus_USB_DataOverRun, /*!< The amount of data returned by the endpoint exceeded
                                     either the size of the maximum data packet allowed
                                     from the endpoint or the remaining buffer size. */
} usb_status_t;

/*! @brief USB host handle type define */
typedef void *usb_host_handle;

/*! @brief USB device handle type define. For device stack it is the whole device handle; for host stack it is the
 * attached device instance handle*/
typedef void *usb_device_handle;

/*! @brief USB OTG handle type define */
typedef void *usb_otg_handle;

/*! @brief USB controller ID */
typedef enum _usb_controller_index
{
  kUSB_ControllerKhci0 = 0U, /*!< KHCI 0U */
  kUSB_ControllerKhci1 = 1U, /*!< KHCI 1U, Currently, there are no platforms which have two KHCI IPs, this is reserved
                                  to be used in the future. */
  kUSB_ControllerEhci0 = 2U, /*!< EHCI 0U */
  kUSB_ControllerEhci1 = 3U, /*!< EHCI 1U, Currently, there are no platforms which have two EHCI IPs, this is reserved
                                  to be used in the future. */

  kUSB_ControllerLpcIp3511Fs0 = 4U, /*!< LPC USB IP3511 FS controller 0 */
  kUSB_ControllerLpcIp3511Fs1 = 5U, /*!< LPC USB IP3511 FS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
              to be used in the future. */

  kUSB_ControllerLpcIp3511Hs0 = 6U, /*!< LPC USB IP3511 HS controller 0 */
  kUSB_ControllerLpcIp3511Hs1 = 7U, /*!< LPC USB IP3511 HS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
              to be used in the future. */

  kUSB_ControllerOhci0 = 8U, /*!< OHCI 0U */
  kUSB_ControllerOhci1 = 9U, /*!< OHCI 1U, Currently, there are no platforms which have two OHCI IPs, this is reserved
                                  to be used in the future. */

  kUSB_ControllerIp3516Hs0 = 10U, /*!< IP3516HS 0U */
  kUSB_ControllerIp3516Hs1 = 11U, /*!< IP3516HS 1U, Currently, there are no platforms which have two IP3516HS IPs, this is reserved
           to be used in the future. */
  kUSB_ControllerDwc30     = 12U, /*!< DWC3 0U */
  kUSB_ControllerDwc31     = 13U, /*!< DWC3 1U Currently, there are no platforms which have two Dwc IPs, this is reserved
           to be used in the future.*/
} usb_controller_index_t;

/**
* @brief USB stack version fields
*/
typedef struct _usb_version
{
  uint8_t major;  /*!< Major */
  uint8_t minor;  /*!< Minor */
  uint8_t bugfix; /*!< Bug fix */
} usb_version_t;

//-------------------------------------------------------------------------------------------

void *USB_EhciPhyGetBase(uint8_t controllerId)
{
  void *   usbPhyBase = NULL;
  uint32_t instance;
  uint32_t newinstance        = 0;
  uint32_t usbphy_base_temp[] = USBPHY_BASE_ADDRS;
  uint32_t usbphy_base[]      = USBPHY_BASE_ADDRS;

  if(controllerId < kUSB_ControllerEhci0)
  {
    return NULL;
  }

  if((controllerId == kUSB_ControllerEhci0) || (controllerId == kUSB_ControllerEhci1))
  {
    controllerId = controllerId - kUSB_ControllerEhci0;
  }
  else if((controllerId == kUSB_ControllerLpcIp3511Hs0) || (controllerId == kUSB_ControllerLpcIp3511Hs1))
  {
    controllerId = controllerId - kUSB_ControllerLpcIp3511Hs0;
  }
  else if((controllerId == kUSB_ControllerIp3516Hs0) || (controllerId == kUSB_ControllerIp3516Hs1))
  {
    controllerId = controllerId - kUSB_ControllerIp3516Hs0;
  }
  else
  {
  }

  for(instance = 0; instance < (sizeof(usbphy_base_temp) / sizeof(usbphy_base_temp[0])); instance++)
  {
    if(usbphy_base_temp[instance])
    {
      usbphy_base[newinstance++] = usbphy_base_temp[instance];
    }
  }
  if(controllerId > newinstance)
  {
    return NULL;
  }

  usbPhyBase = (void *)usbphy_base[controllerId];
  return usbPhyBase;
}

/*!
 * @brief ehci phy initialization.
 *
 * This function initialize ehci phy IP.
 *
 * @param[in] controllerId   ehci controller id, please reference to #usb_controller_index_t.
 * @param[in] freq            the external input clock.
 *                            for example: if the external input clock is 16M, the parameter freq should be 16000000.
 *
 * @retval kStatus_USB_Success      cancel successfully.
 * @retval kStatus_USB_Error        the freq value is incorrect.
 */
uint32_t USB_EhciPhyInit(uint8_t controllerId, uint32_t freq, usb_phy_config_struct_t *phyConfig)
{
  USBPHY_Type *usbPhyBase;

  usbPhyBase = (USBPHY_Type *)USB_EhciPhyGetBase(controllerId);
  if(NULL == usbPhyBase)
  {
    return kStatus_USB_Error;
  }

  USB_ANALOG->INSTANCE[controllerId - kUSB_ControllerEhci0].CHRG_DETECT_SET = USB_ANALOG_CHRG_DETECT_CHK_CHRG_B(1) | USB_ANALOG_CHRG_DETECT_EN_B(1);

  usbPhyBase->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK; /* support LS device. */
  usbPhyBase->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL3_MASK; /* support external FS Hub with LS device connected. */
  /* PWD register provides overall control of the PHY power state */
  usbPhyBase->PWD = 0U;
  if((kUSB_ControllerIp3516Hs0 == controllerId) || (kUSB_ControllerIp3516Hs1 == controllerId) || (kUSB_ControllerLpcIp3511Hs0 == controllerId)
     || (kUSB_ControllerLpcIp3511Hs0 == controllerId))
  {
    usbPhyBase->CTRL_SET = USBPHY_CTRL_SET_ENAUTOCLR_CLKGATE_MASK;
    usbPhyBase->CTRL_SET = USBPHY_CTRL_SET_ENAUTOCLR_PHY_PWD_MASK;
  }
  if(NULL != phyConfig)
  {
    /* Decode to trim the nominal 17.78mA current source for the High Speed TX drivers on USB_DP and USB_DM. */
    usbPhyBase->TX = ((usbPhyBase->TX & (~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK | USBPHY_TX_TXCAL45DP_MASK)))
                      | (USBPHY_TX_D_CAL(phyConfig->D_CAL) | USBPHY_TX_TXCAL45DP(phyConfig->TXCAL45DP) | USBPHY_TX_TXCAL45DM(phyConfig->TXCAL45DM)));
  }

  return kStatus_USB_Success;
}
