/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __USB_PHY_H__
#define __USB_PHY_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _usb_phy_config_struct
{
  uint8_t D_CAL;     /* Decode to trim the nominal 17.78mA current source */
  uint8_t TXCAL45DP; /* Decode to trim the nominal 45-Ohm series termination resistance to the USB_DP output pin */
  uint8_t TXCAL45DM; /* Decode to trim the nominal 45-Ohm series termination resistance to the USB_DM output pin */
} usb_phy_config_struct_t;

#if defined(__cplusplus)
extern "C"
{
#endif

  /*******************************************************************************
 * API
 ******************************************************************************/
  /*!
 * @brief EHCI PHY get USB phy bass address.
 *
 * This function is used to get USB phy bass address.
 *
 * @param[in] controllerId    EHCI controller ID; See the #usb_controller_index_t.
 *
 * @retval USB phy bass address.
 */
  extern void *USB_EhciPhyGetBase(uint8_t controllerId);

  /*!
 * @brief EHCI PHY initialization.
 *
 * This function initializes the EHCI PHY IP.
 *
 * @param[in] controllerId    EHCI controller ID; See the #usb_controller_index_t.
 * @param[in] freq            The external input clock.
 *
 * @retval kStatus_USB_Success      Cancel successfully.
 * @retval kStatus_USB_Error        The freq value is incorrect.
 */
  extern uint32_t USB_EhciPhyInit(uint8_t controllerId, uint32_t freq, usb_phy_config_struct_t *phyConfig);

#if defined(__cplusplus)
}
#endif

#endif /* __USB_PHY_H__ */
