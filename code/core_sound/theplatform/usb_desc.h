#pragma once

#include <stddef.h>
#include <stdint.h>

#define ENDPOINT_TRANSMIT_UNUSED      0x00020000
#define ENDPOINT_TRANSMIT_ISOCHRONOUS 0x00C40000
#define ENDPOINT_TRANSMIT_BULK        0x00C80000
#define ENDPOINT_TRANSMIT_INTERRUPT   0x00CC0000
#define ENDPOINT_RECEIVE_UNUSED       0x00000002
#define ENDPOINT_RECEIVE_ISOCHRONOUS  0x000000C4
#define ENDPOINT_RECEIVE_BULK         0x000000C8
#define ENDPOINT_RECEIVE_INTERRUPT    0x000000CC

#define VENDOR_ID  0x1208
#define PRODUCT_ID 0x0481
#define MANUFACTURER_NAME                                                                                             \
  {                                                                                                                   \
    'S', 'h', 'e', 'z', 'l', 'o', 'n', 'g', ' ', 'S', 'e', 'm', 'i', 'c', 'o', 'n', 'd', 'u', 'c', 't', 'o', 'r', 's' \
  }
#define MANUFACTURER_NAME_LEN 23
#define PRODUCT_NAME                             \
  {                                              \
    'S', 'h', 'e', 'z', ' ', 'M', 'I', 'D', 'I', \
  }
#define PRODUCT_NAME_LEN 9
#define EP0_SIZE         64
#define NUM_ENDPOINTS    4
#define NUM_INTERFACE    2

#define MIDI_INTERFACE   0
#define MIDI_NUM_CABLES  1
#define MIDI_TX_ENDPOINT 3
#define MIDI_TX_SIZE_12  64
#define MIDI_TX_SIZE_480 512
#define MIDI_RX_ENDPOINT 3
#define MIDI_RX_SIZE_12  64
#define MIDI_RX_SIZE_480 512
#define ENDPOINT2_CONFIG ENDPOINT_RECEIVE_INTERRUPT + ENDPOINT_TRANSMIT_INTERRUPT
#define ENDPOINT3_CONFIG ENDPOINT_RECEIVE_BULK + ENDPOINT_TRANSMIT_BULK

#ifdef USB_DESC_LIST_DEFINE
#if defined(NUM_ENDPOINTS) && NUM_ENDPOINTS > 0

extern const uint32_t usb_endpoint_config_table[NUM_ENDPOINTS];

typedef struct
{
  uint16_t       wValue;
  uint16_t       wIndex;
  const uint8_t *addr;
  uint16_t       length;
} usb_descriptor_list_t;

extern const usb_descriptor_list_t usb_descriptor_list[];
#endif
#endif
