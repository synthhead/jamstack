#ifndef _usb_names_h_
#define _usb_names_h_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct usb_string_descriptor_struct
  {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wString[];
  };

  extern struct usb_string_descriptor_struct usb_string_manufacturer_name;
  extern struct usb_string_descriptor_struct usb_string_product_name;
  extern struct usb_string_descriptor_struct usb_string_serial_number;

#ifdef __cplusplus
}
#endif

#endif
