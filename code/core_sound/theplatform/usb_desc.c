

#define USB_DESC_LIST_DEFINE
#include "usb_desc.h"
#ifdef NUM_ENDPOINTS
#include "usb_names.h"
#include "micro1020.h"

#include "memspace.h"

#include "usb_helpers.h"

#if defined(F_CPU) && F_CPU < 30000000
#undef DMAMEM
#endif

char *ultoa(unsigned long val, char *buf, int radix)
{
  unsigned digit;
  int      i = 0, j;
  char     t;

  while(1)
  {
    digit  = val % radix;
    buf[i] = ((digit < 10) ? '0' + digit : 'A' + digit - 10);
    val /= radix;
    if(val == 0)
      break;
    i++;
  }
  buf[i + 1] = 0;
  for(j = 0; j < i; j++, i--)
  {
    t      = buf[j];
    buf[j] = buf[i];
    buf[i] = t;
  }
  return buf;
}

#define LSB(n) ((n)&255)
#define MSB(n) (((n) >> 8) & 255)

#ifdef CDC_IAD_DESCRIPTOR
#ifndef DEVICE_CLASS
#define DEVICE_CLASS 0xEF
#endif
#ifndef DEVICE_SUBCLASS
#define DEVICE_SUBCLASS 0x02
#endif
#ifndef DEVICE_PROTOCOL
#define DEVICE_PROTOCOL 0x01
#endif
#endif

static uint8_t device_descriptor[] = {18,
                                      1,
                                      0x00,
                                      0x02,
#ifdef DEVICE_CLASS
                                      DEVICE_CLASS,
#else
                                      0,
#endif
#ifdef DEVICE_SUBCLASS
                                      DEVICE_SUBCLASS,
#else
                                      0,
#endif
#ifdef DEVICE_PROTOCOL
                                      DEVICE_PROTOCOL,
#else
                                      0,
#endif
                                      EP0_SIZE,
                                      LSB(VENDOR_ID),
                                      MSB(VENDOR_ID),
                                      LSB(PRODUCT_ID),
                                      MSB(PRODUCT_ID),
#ifdef BCD_DEVICE
                                      LSB(BCD_DEVICE),
                                      MSB(BCD_DEVICE),
#else
                                      0x79,
                                      0x02,

#endif
                                      1,
                                      2,
                                      3,
                                      1};

FLASHMEM static const uint8_t qualifier_descriptor[] = {

    10,
    6,
    0x00,
    0x02,
#ifdef DEVICE_CLASS
    DEVICE_CLASS,
#else
    0,
#endif
#ifdef DEVICE_SUBCLASS
    DEVICE_SUBCLASS,
#else
    0,
#endif
#ifdef DEVICE_PROTOCOL
    DEVICE_PROTOCOL,
#else
    0,
#endif
    EP0_SIZE,
    1,
    0};

#define CONFIG_HEADER_DESCRIPTOR_SIZE 9

#define CDC_IAD_DESCRIPTOR_POS CONFIG_HEADER_DESCRIPTOR_SIZE
#ifdef CDC_IAD_DESCRIPTOR
#define CDC_IAD_DESCRIPTOR_SIZE 8
#else
#define CDC_IAD_DESCRIPTOR_SIZE 0
#endif

#define CDC_DATA_INTERFACE_DESC_POS CDC_IAD_DESCRIPTOR_POS + CDC_IAD_DESCRIPTOR_SIZE
#ifdef CDC_DATA_INTERFACE
#define CDC_DATA_INTERFACE_DESC_SIZE 9 + 5 + 5 + 4 + 5 + 7 + 9 + 7 + 7
#else
#define CDC_DATA_INTERFACE_DESC_SIZE 0
#endif

#define CDC2_DATA_INTERFACE_DESC_POS CDC_DATA_INTERFACE_DESC_POS + CDC_DATA_INTERFACE_DESC_SIZE
#ifdef CDC2_DATA_INTERFACE
#define CDC2_DATA_INTERFACE_DESC_SIZE 8 + 9 + 5 + 5 + 4 + 5 + 7 + 9 + 7 + 7
#else
#define CDC2_DATA_INTERFACE_DESC_SIZE 0
#endif

#define CDC3_DATA_INTERFACE_DESC_POS CDC2_DATA_INTERFACE_DESC_POS + CDC2_DATA_INTERFACE_DESC_SIZE
#ifdef CDC3_DATA_INTERFACE
#define CDC3_DATA_INTERFACE_DESC_SIZE 8 + 9 + 5 + 5 + 4 + 5 + 7 + 9 + 7 + 7
#else
#define CDC3_DATA_INTERFACE_DESC_SIZE 0
#endif

#define MIDI_INTERFACE_DESC_POS CDC3_DATA_INTERFACE_DESC_POS + CDC3_DATA_INTERFACE_DESC_SIZE
#ifdef MIDI_INTERFACE
#if !defined(MIDI_NUM_CABLES) || MIDI_NUM_CABLES < 1 || MIDI_NUM_CABLES > 16
#error "MIDI_NUM_CABLES must be defined between 1 to 16"
#endif
#define MIDI_INTERFACE_DESC_SIZE 9 + 7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + (9 + 4 + MIDI_NUM_CABLES) * 2
#else
#define MIDI_INTERFACE_DESC_SIZE 0
#endif

#define KEYBOARD_INTERFACE_DESC_POS MIDI_INTERFACE_DESC_POS + MIDI_INTERFACE_DESC_SIZE
#ifdef KEYBOARD_INTERFACE
#define KEYBOARD_INTERFACE_DESC_SIZE 9 + 9 + 7
#define KEYBOARD_HID_DESC_OFFSET     KEYBOARD_INTERFACE_DESC_POS + 9
#else
#define KEYBOARD_INTERFACE_DESC_SIZE 0
#endif

#define MOUSE_INTERFACE_DESC_POS KEYBOARD_INTERFACE_DESC_POS + KEYBOARD_INTERFACE_DESC_SIZE
#ifdef MOUSE_INTERFACE
#define MOUSE_INTERFACE_DESC_SIZE 9 + 9 + 7
#define MOUSE_HID_DESC_OFFSET     MOUSE_INTERFACE_DESC_POS + 9
#else
#define MOUSE_INTERFACE_DESC_SIZE 0
#endif

#define RAWHID_INTERFACE_DESC_POS MOUSE_INTERFACE_DESC_POS + MOUSE_INTERFACE_DESC_SIZE
#ifdef RAWHID_INTERFACE
#define RAWHID_INTERFACE_DESC_SIZE 9 + 9 + 7 + 7
#define RAWHID_HID_DESC_OFFSET     RAWHID_INTERFACE_DESC_POS + 9
#else
#define RAWHID_INTERFACE_DESC_SIZE 0
#endif

#define FLIGHTSIM_INTERFACE_DESC_POS RAWHID_INTERFACE_DESC_POS + RAWHID_INTERFACE_DESC_SIZE
#ifdef FLIGHTSIM_INTERFACE
#define FLIGHTSIM_INTERFACE_DESC_SIZE 9 + 9 + 7 + 7
#define FLIGHTSIM_HID_DESC_OFFSET     FLIGHTSIM_INTERFACE_DESC_POS + 9
#else
#define FLIGHTSIM_INTERFACE_DESC_SIZE 0
#endif

#define SEREMU_INTERFACE_DESC_POS FLIGHTSIM_INTERFACE_DESC_POS + FLIGHTSIM_INTERFACE_DESC_SIZE
#ifdef SEREMU_INTERFACE
#define SEREMU_INTERFACE_DESC_SIZE 9 + 9 + 7 + 7
#define SEREMU_HID_DESC_OFFSET     SEREMU_INTERFACE_DESC_POS + 9
#else
#define SEREMU_INTERFACE_DESC_SIZE 0
#endif

#define JOYSTICK_INTERFACE_DESC_POS SEREMU_INTERFACE_DESC_POS + SEREMU_INTERFACE_DESC_SIZE
#ifdef JOYSTICK_INTERFACE
#define JOYSTICK_INTERFACE_DESC_SIZE 9 + 9 + 7
#define JOYSTICK_HID_DESC_OFFSET     JOYSTICK_INTERFACE_DESC_POS + 9
#else
#define JOYSTICK_INTERFACE_DESC_SIZE 0
#endif

#define MTP_INTERFACE_DESC_POS JOYSTICK_INTERFACE_DESC_POS + JOYSTICK_INTERFACE_DESC_SIZE
#ifdef MTP_INTERFACE
#define MTP_INTERFACE_DESC_SIZE 9 + 7 + 7 + 7
#else
#define MTP_INTERFACE_DESC_SIZE 0
#endif

#define KEYMEDIA_INTERFACE_DESC_POS MTP_INTERFACE_DESC_POS + MTP_INTERFACE_DESC_SIZE
#ifdef KEYMEDIA_INTERFACE
#define KEYMEDIA_INTERFACE_DESC_SIZE 9 + 9 + 7
#define KEYMEDIA_HID_DESC_OFFSET     KEYMEDIA_INTERFACE_DESC_POS + 9
#else
#define KEYMEDIA_INTERFACE_DESC_SIZE 0
#endif

#define AUDIO_INTERFACE_DESC_POS KEYMEDIA_INTERFACE_DESC_POS + KEYMEDIA_INTERFACE_DESC_SIZE
#ifdef AUDIO_INTERFACE
#define AUDIO_INTERFACE_DESC_SIZE \
  8 + 9 + 10 + 12 + 9 + 12 + 10 + 9 + 9 + 9 + 7 + 11 + 9 + 7 + 9 + 9 + 7 + 11 + 9 + 7 + 9
#else
#define AUDIO_INTERFACE_DESC_SIZE 0
#endif

#define MULTITOUCH_INTERFACE_DESC_POS AUDIO_INTERFACE_DESC_POS + AUDIO_INTERFACE_DESC_SIZE
#ifdef MULTITOUCH_INTERFACE
#define MULTITOUCH_INTERFACE_DESC_SIZE 9 + 9 + 7
#define MULTITOUCH_HID_DESC_OFFSET     MULTITOUCH_INTERFACE_DESC_POS + 9
#else
#define MULTITOUCH_INTERFACE_DESC_SIZE 0
#endif

#define CONFIG_DESC_SIZE MULTITOUCH_INTERFACE_DESC_POS + MULTITOUCH_INTERFACE_DESC_SIZE

FLASHMEM const uint8_t usb_config_descriptor_480[CONFIG_DESC_SIZE] = {

    9,
    2,
    LSB(CONFIG_DESC_SIZE),
    MSB(CONFIG_DESC_SIZE),
    NUM_INTERFACE,
    1,
    0,
    0xC0,
    50,

#ifdef CDC_IAD_DESCRIPTOR

    8,
    11,
    CDC_STATUS_INTERFACE,
    2,
    0x02,
    0x02,
    0x01,
    0,
#endif

#ifdef CDC_DATA_INTERFACE

    9,
    4,
    CDC_STATUS_INTERFACE,
    0,
    1,
    0x02,
    0x02,
    0x01,
    0,

    5,
    0x24,
    0x00,
    0x10,
    0x01,

    5,
    0x24,
    0x01,
    0x01,
    1,

    4,
    0x24,
    0x02,
    0x06,

    5,
    0x24,
    0x06,
    CDC_STATUS_INTERFACE,
    CDC_DATA_INTERFACE,

    7,
    5,
    CDC_ACM_ENDPOINT | 0x80,
    0x03,
    LSB(CDC_ACM_SIZE),
    MSB(CDC_ACM_SIZE),
    5,

    9,
    4,
    CDC_DATA_INTERFACE,
    0,
    2,
    0x0A,
    0x00,
    0x00,
    0,

    7,
    5,
    CDC_RX_ENDPOINT,
    0x02,
    LSB(CDC_RX_SIZE_480),
    MSB(CDC_RX_SIZE_480),
    0,

    7,
    5,
    CDC_TX_ENDPOINT | 0x80,
    0x02,
    LSB(CDC_TX_SIZE_480),
    MSB(CDC_TX_SIZE_480),
    0,
#endif

#ifdef CDC2_DATA_INTERFACE

    8,
    11,
    CDC2_STATUS_INTERFACE,
    2,
    0x02,
    0x02,
    0x01,
    0,

    9,
    4,
    CDC2_STATUS_INTERFACE,
    0,
    1,
    0x02,
    0x02,
    0x01,
    0,

    5,
    0x24,
    0x00,
    0x10,
    0x01,

    5,
    0x24,
    0x01,
    0x01,
    1,

    4,
    0x24,
    0x02,
    0x06,

    5,
    0x24,
    0x06,
    CDC2_STATUS_INTERFACE,
    CDC2_DATA_INTERFACE,

    7,
    5,
    CDC2_ACM_ENDPOINT | 0x80,
    0x03,
    CDC_ACM_SIZE,
    0,
    5,

    9,
    4,
    CDC2_DATA_INTERFACE,
    0,
    2,
    0x0A,
    0x00,
    0x00,
    0,

    7,
    5,
    CDC2_RX_ENDPOINT,
    0x02,
    LSB(CDC_RX_SIZE_480),
    MSB(CDC_RX_SIZE_480),
    0,

    7,
    5,
    CDC2_TX_ENDPOINT | 0x80,
    0x02,
    LSB(CDC_TX_SIZE_480),
    MSB(CDC_TX_SIZE_480),
    0,
#endif

#ifdef CDC3_DATA_INTERFACE

    8,
    11,
    CDC3_STATUS_INTERFACE,
    2,
    0x02,
    0x02,
    0x01,
    0,

    9,
    4,
    CDC3_STATUS_INTERFACE,
    0,
    1,
    0x02,
    0x02,
    0x01,
    0,

    5,
    0x24,
    0x00,
    0x10,
    0x01,

    5,
    0x24,
    0x01,
    0x01,
    1,

    4,
    0x24,
    0x02,
    0x06,

    5,
    0x24,
    0x06,
    CDC3_STATUS_INTERFACE,
    CDC3_DATA_INTERFACE,

    7,
    5,
    CDC3_ACM_ENDPOINT | 0x80,
    0x03,
    CDC_ACM_SIZE,
    0,
    5,

    9,
    4,
    CDC3_DATA_INTERFACE,
    0,
    2,
    0x0A,
    0x00,
    0x00,
    0,

    7,
    5,
    CDC3_RX_ENDPOINT,
    0x02,
    LSB(CDC_RX_SIZE_480),
    MSB(CDC_RX_SIZE_480),
    0,

    7,
    5,
    CDC3_TX_ENDPOINT | 0x80,
    0x02,
    LSB(CDC_TX_SIZE_480),
    MSB(CDC_TX_SIZE_480),
    0,
#endif

#ifdef MIDI_INTERFACE

    9,
    4,
    MIDI_INTERFACE,
    0,
    2,
    0x01,
    0x03,
    0x00,
    0,

    7,
    0x24,
    0x01,
    0x00,
    0x01,
    LSB(7 + (6 + 6 + 9 + 9) * MIDI_NUM_CABLES),
    MSB(7 + (6 + 6 + 9 + 9) * MIDI_NUM_CABLES),

    6,
    0x24,
    0x02,
    0x01,
    1,
    0,

    6,
    0x24,
    0x02,
    0x02,
    2,
    0,

    9,
    0x24,
    0x03,
    0x01,
    3,
    1,
    2,
    1,
    0,

    9,
    0x24,
    0x03,
    0x02,
    4,
    1,
    1,
    1,
    0,
#if MIDI_NUM_CABLES >= 2
#define MIDI_INTERFACE_JACK_PAIR(a, b, c, d)                                                                       \
  6, 0x24, 0x02, 0x01, (a), 0, 6, 0x24, 0x02, 0x02, (b), 0, 9, 0x24, 0x03, 0x01, (c), 1, (b), 1, 0, 9, 0x24, 0x03, \
      0x02, (d), 1, (a), 1, 0,
    MIDI_INTERFACE_JACK_PAIR(5, 6, 7, 8)
#endif
#if MIDI_NUM_CABLES >= 3
        MIDI_INTERFACE_JACK_PAIR(9, 10, 11, 12)
#endif
#if MIDI_NUM_CABLES >= 4
            MIDI_INTERFACE_JACK_PAIR(13, 14, 15, 16)
#endif
#if MIDI_NUM_CABLES >= 5
                MIDI_INTERFACE_JACK_PAIR(17, 18, 19, 20)
#endif
#if MIDI_NUM_CABLES >= 6
                    MIDI_INTERFACE_JACK_PAIR(21, 22, 23, 24)
#endif
#if MIDI_NUM_CABLES >= 7
                        MIDI_INTERFACE_JACK_PAIR(25, 26, 27, 28)
#endif
#if MIDI_NUM_CABLES >= 8
                            MIDI_INTERFACE_JACK_PAIR(29, 30, 31, 32)
#endif
#if MIDI_NUM_CABLES >= 9
                                MIDI_INTERFACE_JACK_PAIR(33, 34, 35, 36)
#endif
#if MIDI_NUM_CABLES >= 10
                                    MIDI_INTERFACE_JACK_PAIR(37, 38, 39, 40)
#endif
#if MIDI_NUM_CABLES >= 11
                                        MIDI_INTERFACE_JACK_PAIR(41, 42, 43, 44)
#endif
#if MIDI_NUM_CABLES >= 12
                                            MIDI_INTERFACE_JACK_PAIR(45, 46, 47, 48)
#endif
#if MIDI_NUM_CABLES >= 13
                                                MIDI_INTERFACE_JACK_PAIR(49, 50, 51, 52)
#endif
#if MIDI_NUM_CABLES >= 14
                                                    MIDI_INTERFACE_JACK_PAIR(53, 54, 55, 56)
#endif
#if MIDI_NUM_CABLES >= 15
                                                        MIDI_INTERFACE_JACK_PAIR(57, 58, 59, 60)
#endif
#if MIDI_NUM_CABLES >= 16
                                                            MIDI_INTERFACE_JACK_PAIR(61, 62, 63, 64)
#endif

                                                                9,
    5,
    MIDI_RX_ENDPOINT,
    0x02,
    LSB(MIDI_RX_SIZE_480),
    MSB(MIDI_RX_SIZE_480),
    0,
    0,
    0,

    4 + MIDI_NUM_CABLES,
    0x25,
    0x01,
    MIDI_NUM_CABLES,
    1,
#if MIDI_NUM_CABLES >= 2
    5,
#endif
#if MIDI_NUM_CABLES >= 3
    9,
#endif
#if MIDI_NUM_CABLES >= 4
    13,
#endif
#if MIDI_NUM_CABLES >= 5
    17,
#endif
#if MIDI_NUM_CABLES >= 6
    21,
#endif
#if MIDI_NUM_CABLES >= 7
    25,
#endif
#if MIDI_NUM_CABLES >= 8
    29,
#endif
#if MIDI_NUM_CABLES >= 9
    33,
#endif
#if MIDI_NUM_CABLES >= 10
    37,
#endif
#if MIDI_NUM_CABLES >= 11
    41,
#endif
#if MIDI_NUM_CABLES >= 12
    45,
#endif
#if MIDI_NUM_CABLES >= 13
    49,
#endif
#if MIDI_NUM_CABLES >= 14
    53,
#endif
#if MIDI_NUM_CABLES >= 15
    57,
#endif
#if MIDI_NUM_CABLES >= 16
    61,
#endif

    9,
    5,
    MIDI_TX_ENDPOINT | 0x80,
    0x02,
    LSB(MIDI_TX_SIZE_480),
    MSB(MIDI_TX_SIZE_480),
    0,
    0,
    0,

    4 + MIDI_NUM_CABLES,
    0x25,
    0x01,
    MIDI_NUM_CABLES,
    3,
#if MIDI_NUM_CABLES >= 2
    7,
#endif
#if MIDI_NUM_CABLES >= 3
    11,
#endif
#if MIDI_NUM_CABLES >= 4
    15,
#endif
#if MIDI_NUM_CABLES >= 5
    19,
#endif
#if MIDI_NUM_CABLES >= 6
    23,
#endif
#if MIDI_NUM_CABLES >= 7
    27,
#endif
#if MIDI_NUM_CABLES >= 8
    31,
#endif
#if MIDI_NUM_CABLES >= 9
    35,
#endif
#if MIDI_NUM_CABLES >= 10
    39,
#endif
#if MIDI_NUM_CABLES >= 11
    43,
#endif
#if MIDI_NUM_CABLES >= 12
    47,
#endif
#if MIDI_NUM_CABLES >= 13
    51,
#endif
#if MIDI_NUM_CABLES >= 14
    55,
#endif
#if MIDI_NUM_CABLES >= 15
    59,
#endif
#if MIDI_NUM_CABLES >= 16
    63,
#endif
#endif

#ifdef KEYBOARD_INTERFACE

    9,
    4,
    KEYBOARD_INTERFACE,
    0,
    1,
    0x03,
    0x01,
    0x01,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(keyboard_report_desc)),
    MSB(sizeof(keyboard_report_desc)),

    7,
    5,
    KEYBOARD_ENDPOINT | 0x80,
    0x03,
    KEYBOARD_SIZE,
    0,
    KEYBOARD_INTERVAL,
#endif

#ifdef MOUSE_INTERFACE

    9,
    4,
    MOUSE_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(mouse_report_desc)),
    MSB(sizeof(mouse_report_desc)),

    7,
    5,
    MOUSE_ENDPOINT | 0x80,
    0x03,
    MOUSE_SIZE,
    0,
    MOUSE_INTERVAL,
#endif

#ifdef RAWHID_INTERFACE

    9,
    4,
    RAWHID_INTERFACE,
    0,
    2,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(rawhid_report_desc)),
    MSB(sizeof(rawhid_report_desc)),

    7,
    5,
    RAWHID_TX_ENDPOINT | 0x80,
    0x03,
    RAWHID_TX_SIZE,
    0,
    RAWHID_TX_INTERVAL,

    7,
    5,
    RAWHID_RX_ENDPOINT,
    0x03,
    RAWHID_RX_SIZE,
    0,
    RAWHID_RX_INTERVAL,
#endif

#ifdef FLIGHTSIM_INTERFACE

    9,
    4,
    FLIGHTSIM_INTERFACE,
    0,
    2,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(flightsim_report_desc)),
    MSB(sizeof(flightsim_report_desc)),

    7,
    5,
    FLIGHTSIM_TX_ENDPOINT | 0x80,
    0x03,
    FLIGHTSIM_TX_SIZE,
    0,
    FLIGHTSIM_TX_INTERVAL,

    7,
    5,
    FLIGHTSIM_RX_ENDPOINT,
    0x03,
    FLIGHTSIM_RX_SIZE,
    0,
    FLIGHTSIM_RX_INTERVAL,
#endif

#ifdef SEREMU_INTERFACE

    9,
    4,
    SEREMU_INTERFACE,
    0,
    2,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(seremu_report_desc)),
    MSB(sizeof(seremu_report_desc)),

    7,
    5,
    SEREMU_TX_ENDPOINT | 0x80,
    0x03,
    SEREMU_TX_SIZE,
    0,
    SEREMU_TX_INTERVAL,

    7,
    5,
    SEREMU_RX_ENDPOINT,
    0x03,
    SEREMU_RX_SIZE,
    0,
    SEREMU_RX_INTERVAL,
#endif

#ifdef JOYSTICK_INTERFACE

    9,
    4,
    JOYSTICK_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(joystick_report_desc)),
    MSB(sizeof(joystick_report_desc)),

    7,
    5,
    JOYSTICK_ENDPOINT | 0x80,
    0x03,
    JOYSTICK_SIZE,
    0,
    JOYSTICK_INTERVAL,
#endif

#ifdef MTP_INTERFACE

    9,
    4,
    MTP_INTERFACE,
    0,
    3,
    0x06,
    0x01,
    0x01,
    0,

    7,
    5,
    MTP_TX_ENDPOINT | 0x80,
    0x02,
    LSB(MTP_TX_SIZE_480),
    MSB(MTP_TX_SIZE_480),
    0,

    7,
    5,
    MTP_RX_ENDPOINT,
    0x02,
    LSB(MTP_RX_SIZE_480),
    MSB(MTP_RX_SIZE_480),
    0,

    7,
    5,
    MTP_EVENT_ENDPOINT | 0x80,
    0x03,
    MTP_EVENT_SIZE,
    0,
    MTP_EVENT_INTERVAL_480,
#endif

#ifdef KEYMEDIA_INTERFACE

    9,
    4,
    KEYMEDIA_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(keymedia_report_desc)),
    MSB(sizeof(keymedia_report_desc)),

    7,
    5,
    KEYMEDIA_ENDPOINT | 0x80,
    0x03,
    KEYMEDIA_SIZE,
    0,
    KEYMEDIA_INTERVAL,
#endif

#ifdef AUDIO_INTERFACE

    8,
    11,
    AUDIO_INTERFACE,
    3,
    0x01,
    0x01,
    0x00,
    0,

    9,
    4,
    AUDIO_INTERFACE,
    0,
    0,
    1,
    1,
    0,
    0,

    10,
    0x24,
    0x01,
    0x00,
    0x01,
    LSB(62),
    MSB(62),
    2,
    AUDIO_INTERFACE + 1,
    AUDIO_INTERFACE + 2,

    12,
    0x24,
    0x02,
    1,

    0x02,
    0x06,
    0,
    2,
    0x03,
    0x00,
    0,
    0,

    9,
    0x24,
    3,
    2,
    0x01,
    0x01,
    0,
    1,
    0,

    12,
    0x24,
    2,
    3,
    0x01,
    0x01,
    0,
    2,
    0x03,
    0x00,
    0,
    0,

    10,
    0x24,
    0x06,
    0x31,
    0x03,
    0x01,
    0x01,
    0x02,
    0x02,
    0x00,

    9,
    0x24,
    3,
    4,

    0x02,
    0x06,
    0,
    0x31,
    0,

    9,
    4,
    AUDIO_INTERFACE + 1,
    0,
    0,
    1,
    2,
    0,
    0,

    9,
    4,
    AUDIO_INTERFACE + 1,
    1,
    1,
    1,
    2,
    0,
    0,

    7,
    0x24,
    1,
    2,
    3,
    0x01,
    0x00,

    11,
    0x24,
    2,
    1,
    2,
    2,
    16,
    1,
    LSB(44100),
    MSB(44100),
    0,

    9,
    5,
    AUDIO_TX_ENDPOINT | 0x80,
    0x09,
    LSB(AUDIO_TX_SIZE),
    MSB(AUDIO_TX_SIZE),
    4,
    0,
    0,

    7,
    0x25,
    1,
    0x00,
    0,
    0x00,
    0x00,

    9,
    4,
    AUDIO_INTERFACE + 2,
    0,
    0,
    1,
    2,
    0,
    0,

    9,
    4,
    AUDIO_INTERFACE + 2,
    1,
    2,
    1,
    2,
    0,
    0,

    7,
    0x24,
    1,
    3,
    3,
    0x01,
    0x00,

    11,
    0x24,
    2,
    1,
    2,
    2,
    16,
    1,
    LSB(44100),
    MSB(44100),
    0,

    9,
    5,
    AUDIO_RX_ENDPOINT,
    0x05,
    LSB(AUDIO_RX_SIZE),
    MSB(AUDIO_RX_SIZE),
    4,
    0,
    AUDIO_SYNC_ENDPOINT | 0x80,

    7,
    0x25,
    1,
    0x00,
    0,
    0x00,
    0x00,

    9,
    5,
    AUDIO_SYNC_ENDPOINT | 0x80,
    0x11,
    4,
    0,
    4,
    7,
    0,
#endif

#ifdef MULTITOUCH_INTERFACE

    9,
    4,
    MULTITOUCH_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(multitouch_report_desc)),
    MSB(sizeof(multitouch_report_desc)),

    7,
    5,
    MULTITOUCH_ENDPOINT | 0x80,
    0x03,
    MULTITOUCH_SIZE,
    0,
    4,
#endif
};

FLASHMEM const uint8_t usb_config_descriptor_12[CONFIG_DESC_SIZE] = {

    9,
    2,
    LSB(CONFIG_DESC_SIZE),
    MSB(CONFIG_DESC_SIZE),
    NUM_INTERFACE,
    1,
    0,
    0xC0,
    50,

#ifdef CDC_IAD_DESCRIPTOR

    8,
    11,
    CDC_STATUS_INTERFACE,
    2,
    0x02,
    0x02,
    0x01,
    0,
#endif

#ifdef CDC_DATA_INTERFACE

    9,
    4,
    CDC_STATUS_INTERFACE,
    0,
    1,
    0x02,
    0x02,
    0x01,
    0,

    5,
    0x24,
    0x00,
    0x10,
    0x01,

    5,
    0x24,
    0x01,
    0x01,
    1,

    4,
    0x24,
    0x02,
    0x06,

    5,
    0x24,
    0x06,
    CDC_STATUS_INTERFACE,
    CDC_DATA_INTERFACE,

    7,
    5,
    CDC_ACM_ENDPOINT | 0x80,
    0x03,
    CDC_ACM_SIZE,
    0,
    16,

    9,
    4,
    CDC_DATA_INTERFACE,
    0,
    2,
    0x0A,
    0x00,
    0x00,
    0,

    7,
    5,
    CDC_RX_ENDPOINT,
    0x02,
    LSB(CDC_RX_SIZE_12),
    MSB(CDC_RX_SIZE_12),
    0,

    7,
    5,
    CDC_TX_ENDPOINT | 0x80,
    0x02,
    LSB(CDC_TX_SIZE_12),
    MSB(CDC_TX_SIZE_12),
    0,
#endif

#ifdef CDC2_DATA_INTERFACE

    8,
    11,
    CDC2_STATUS_INTERFACE,
    2,
    0x02,
    0x02,
    0x01,
    0,

    9,
    4,
    CDC2_STATUS_INTERFACE,
    0,
    1,
    0x02,
    0x02,
    0x01,
    0,

    5,
    0x24,
    0x00,
    0x10,
    0x01,

    5,
    0x24,
    0x01,
    0x01,
    1,

    4,
    0x24,
    0x02,
    0x06,

    5,
    0x24,
    0x06,
    CDC2_STATUS_INTERFACE,
    CDC2_DATA_INTERFACE,

    7,
    5,
    CDC2_ACM_ENDPOINT | 0x80,
    0x03,
    CDC_ACM_SIZE,
    0,
    64,

    9,
    4,
    CDC2_DATA_INTERFACE,
    0,
    2,
    0x0A,
    0x00,
    0x00,
    0,

    7,
    5,
    CDC2_RX_ENDPOINT,
    0x02,
    CDC_RX_SIZE_12,
    0,
    0,

    7,
    5,
    CDC2_TX_ENDPOINT | 0x80,
    0x02,
    CDC_TX_SIZE_12,
    0,
    0,
#endif

#ifdef CDC3_DATA_INTERFACE

    8,
    11,
    CDC3_STATUS_INTERFACE,
    2,
    0x02,
    0x02,
    0x01,
    0,

    9,
    4,
    CDC3_STATUS_INTERFACE,
    0,
    1,
    0x02,
    0x02,
    0x01,
    0,

    5,
    0x24,
    0x00,
    0x10,
    0x01,

    5,
    0x24,
    0x01,
    0x01,
    1,

    4,
    0x24,
    0x02,
    0x06,

    5,
    0x24,
    0x06,
    CDC3_STATUS_INTERFACE,
    CDC3_DATA_INTERFACE,

    7,
    5,
    CDC3_ACM_ENDPOINT | 0x80,
    0x03,
    CDC_ACM_SIZE,
    0,
    64,

    9,
    4,
    CDC3_DATA_INTERFACE,
    0,
    2,
    0x0A,
    0x00,
    0x00,
    0,

    7,
    5,
    CDC3_RX_ENDPOINT,
    0x02,
    CDC_RX_SIZE_12,
    0,
    0,

    7,
    5,
    CDC3_TX_ENDPOINT | 0x80,
    0x02,
    CDC_TX_SIZE_12,
    0,
    0,
#endif

#ifdef MIDI_INTERFACE

    9,
    4,
    MIDI_INTERFACE,
    0,
    2,
    0x01,
    0x03,
    0x00,
    0,

    7,
    0x24,
    0x01,
    0x00,
    0x01,
    LSB(7 + (6 + 6 + 9 + 9) * MIDI_NUM_CABLES),
    MSB(7 + (6 + 6 + 9 + 9) * MIDI_NUM_CABLES),

    6,
    0x24,
    0x02,
    0x01,
    1,
    0,

    6,
    0x24,
    0x02,
    0x02,
    2,
    0,

    9,
    0x24,
    0x03,
    0x01,
    3,
    1,
    2,
    1,
    0,

    9,
    0x24,
    0x03,
    0x02,
    4,
    1,
    1,
    1,
    0,
#if MIDI_NUM_CABLES >= 2
#define MIDI_INTERFACE_JACK_PAIR(a, b, c, d)                                                                       \
  6, 0x24, 0x02, 0x01, (a), 0, 6, 0x24, 0x02, 0x02, (b), 0, 9, 0x24, 0x03, 0x01, (c), 1, (b), 1, 0, 9, 0x24, 0x03, \
      0x02, (d), 1, (a), 1, 0,
    MIDI_INTERFACE_JACK_PAIR(5, 6, 7, 8)
#endif
#if MIDI_NUM_CABLES >= 3
        MIDI_INTERFACE_JACK_PAIR(9, 10, 11, 12)
#endif
#if MIDI_NUM_CABLES >= 4
            MIDI_INTERFACE_JACK_PAIR(13, 14, 15, 16)
#endif
#if MIDI_NUM_CABLES >= 5
                MIDI_INTERFACE_JACK_PAIR(17, 18, 19, 20)
#endif
#if MIDI_NUM_CABLES >= 6
                    MIDI_INTERFACE_JACK_PAIR(21, 22, 23, 24)
#endif
#if MIDI_NUM_CABLES >= 7
                        MIDI_INTERFACE_JACK_PAIR(25, 26, 27, 28)
#endif
#if MIDI_NUM_CABLES >= 8
                            MIDI_INTERFACE_JACK_PAIR(29, 30, 31, 32)
#endif
#if MIDI_NUM_CABLES >= 9
                                MIDI_INTERFACE_JACK_PAIR(33, 34, 35, 36)
#endif
#if MIDI_NUM_CABLES >= 10
                                    MIDI_INTERFACE_JACK_PAIR(37, 38, 39, 40)
#endif
#if MIDI_NUM_CABLES >= 11
                                        MIDI_INTERFACE_JACK_PAIR(41, 42, 43, 44)
#endif
#if MIDI_NUM_CABLES >= 12
                                            MIDI_INTERFACE_JACK_PAIR(45, 46, 47, 48)
#endif
#if MIDI_NUM_CABLES >= 13
                                                MIDI_INTERFACE_JACK_PAIR(49, 50, 51, 52)
#endif
#if MIDI_NUM_CABLES >= 14
                                                    MIDI_INTERFACE_JACK_PAIR(53, 54, 55, 56)
#endif
#if MIDI_NUM_CABLES >= 15
                                                        MIDI_INTERFACE_JACK_PAIR(57, 58, 59, 60)
#endif
#if MIDI_NUM_CABLES >= 16
                                                            MIDI_INTERFACE_JACK_PAIR(61, 62, 63, 64)
#endif

                                                                9,
    5,
    MIDI_RX_ENDPOINT,
    0x02,
    LSB(MIDI_RX_SIZE_12),
    MSB(MIDI_RX_SIZE_12),
    0,
    0,
    0,

    4 + MIDI_NUM_CABLES,
    0x25,
    0x01,
    MIDI_NUM_CABLES,
    1,
#if MIDI_NUM_CABLES >= 2
    5,
#endif
#if MIDI_NUM_CABLES >= 3
    9,
#endif
#if MIDI_NUM_CABLES >= 4
    13,
#endif
#if MIDI_NUM_CABLES >= 5
    17,
#endif
#if MIDI_NUM_CABLES >= 6
    21,
#endif
#if MIDI_NUM_CABLES >= 7
    25,
#endif
#if MIDI_NUM_CABLES >= 8
    29,
#endif
#if MIDI_NUM_CABLES >= 9
    33,
#endif
#if MIDI_NUM_CABLES >= 10
    37,
#endif
#if MIDI_NUM_CABLES >= 11
    41,
#endif
#if MIDI_NUM_CABLES >= 12
    45,
#endif
#if MIDI_NUM_CABLES >= 13
    49,
#endif
#if MIDI_NUM_CABLES >= 14
    53,
#endif
#if MIDI_NUM_CABLES >= 15
    57,
#endif
#if MIDI_NUM_CABLES >= 16
    61,
#endif

    9,
    5,
    MIDI_TX_ENDPOINT | 0x80,
    0x02,
    LSB(MIDI_TX_SIZE_12),
    MSB(MIDI_TX_SIZE_12),
    0,
    0,
    0,

    4 + MIDI_NUM_CABLES,
    0x25,
    0x01,
    MIDI_NUM_CABLES,
    3,
#if MIDI_NUM_CABLES >= 2
    7,
#endif
#if MIDI_NUM_CABLES >= 3
    11,
#endif
#if MIDI_NUM_CABLES >= 4
    15,
#endif
#if MIDI_NUM_CABLES >= 5
    19,
#endif
#if MIDI_NUM_CABLES >= 6
    23,
#endif
#if MIDI_NUM_CABLES >= 7
    27,
#endif
#if MIDI_NUM_CABLES >= 8
    31,
#endif
#if MIDI_NUM_CABLES >= 9
    35,
#endif
#if MIDI_NUM_CABLES >= 10
    39,
#endif
#if MIDI_NUM_CABLES >= 11
    43,
#endif
#if MIDI_NUM_CABLES >= 12
    47,
#endif
#if MIDI_NUM_CABLES >= 13
    51,
#endif
#if MIDI_NUM_CABLES >= 14
    55,
#endif
#if MIDI_NUM_CABLES >= 15
    59,
#endif
#if MIDI_NUM_CABLES >= 16
    63,
#endif
#endif

#ifdef KEYBOARD_INTERFACE

    9,
    4,
    KEYBOARD_INTERFACE,
    0,
    1,
    0x03,
    0x01,
    0x01,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(keyboard_report_desc)),
    MSB(sizeof(keyboard_report_desc)),

    7,
    5,
    KEYBOARD_ENDPOINT | 0x80,
    0x03,
    KEYBOARD_SIZE,
    0,
    KEYBOARD_INTERVAL,
#endif

#ifdef MOUSE_INTERFACE

    9,
    4,
    MOUSE_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(mouse_report_desc)),
    MSB(sizeof(mouse_report_desc)),

    7,
    5,
    MOUSE_ENDPOINT | 0x80,
    0x03,
    MOUSE_SIZE,
    0,
    MOUSE_INTERVAL,
#endif

#ifdef RAWHID_INTERFACE

    9,
    4,
    RAWHID_INTERFACE,
    0,
    2,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(rawhid_report_desc)),
    MSB(sizeof(rawhid_report_desc)),

    7,
    5,
    RAWHID_TX_ENDPOINT | 0x80,
    0x03,
    RAWHID_TX_SIZE,
    0,
    RAWHID_TX_INTERVAL,

    7,
    5,
    RAWHID_RX_ENDPOINT,
    0x03,
    RAWHID_RX_SIZE,
    0,
    RAWHID_RX_INTERVAL,
#endif

#ifdef FLIGHTSIM_INTERFACE

    9,
    4,
    FLIGHTSIM_INTERFACE,
    0,
    2,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(flightsim_report_desc)),
    MSB(sizeof(flightsim_report_desc)),

    7,
    5,
    FLIGHTSIM_TX_ENDPOINT | 0x80,
    0x03,
    FLIGHTSIM_TX_SIZE,
    0,
    FLIGHTSIM_TX_INTERVAL,

    7,
    5,
    FLIGHTSIM_RX_ENDPOINT,
    0x03,
    FLIGHTSIM_RX_SIZE,
    0,
    FLIGHTSIM_RX_INTERVAL,
#endif

#ifdef SEREMU_INTERFACE

    9,
    4,
    SEREMU_INTERFACE,
    0,
    2,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(seremu_report_desc)),
    MSB(sizeof(seremu_report_desc)),

    7,
    5,
    SEREMU_TX_ENDPOINT | 0x80,
    0x03,
    SEREMU_TX_SIZE,
    0,
    SEREMU_TX_INTERVAL,

    7,
    5,
    SEREMU_RX_ENDPOINT,
    0x03,
    SEREMU_RX_SIZE,
    0,
    SEREMU_RX_INTERVAL,
#endif

#ifdef JOYSTICK_INTERFACE

    9,
    4,
    JOYSTICK_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(joystick_report_desc)),
    MSB(sizeof(joystick_report_desc)),

    7,
    5,
    JOYSTICK_ENDPOINT | 0x80,
    0x03,
    JOYSTICK_SIZE,
    0,
    JOYSTICK_INTERVAL,
#endif

#ifdef MTP_INTERFACE

    9,
    4,
    MTP_INTERFACE,
    0,
    3,
    0x06,
    0x01,
    0x01,
    0,

    7,
    5,
    MTP_TX_ENDPOINT | 0x80,
    0x02,
    LSB(MTP_TX_SIZE_12),
    MSB(MTP_TX_SIZE_12),
    0,

    7,
    5,
    MTP_RX_ENDPOINT,
    0x02,
    LSB(MTP_RX_SIZE_12),
    MSB(MTP_RX_SIZE_12),
    0,

    7,
    5,
    MTP_EVENT_ENDPOINT | 0x80,
    0x03,
    MTP_EVENT_SIZE,
    0,
    MTP_EVENT_INTERVAL_12,
#endif

#ifdef KEYMEDIA_INTERFACE

    9,
    4,
    KEYMEDIA_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(keymedia_report_desc)),
    MSB(sizeof(keymedia_report_desc)),

    7,
    5,
    KEYMEDIA_ENDPOINT | 0x80,
    0x03,
    KEYMEDIA_SIZE,
    0,
    KEYMEDIA_INTERVAL,
#endif

#ifdef AUDIO_INTERFACE

    8,
    11,
    AUDIO_INTERFACE,
    3,
    0x01,
    0x01,
    0x00,
    0,

    9,
    4,
    AUDIO_INTERFACE,
    0,
    0,
    1,
    1,
    0,
    0,

    10,
    0x24,
    0x01,
    0x00,
    0x01,
    LSB(62),
    MSB(62),
    2,
    AUDIO_INTERFACE + 1,
    AUDIO_INTERFACE + 2,

    12,
    0x24,
    0x02,
    1,

    0x02,
    0x06,
    0,
    2,
    0x03,
    0x00,
    0,
    0,

    9,
    0x24,
    3,
    2,
    0x01,
    0x01,
    0,
    1,
    0,

    12,
    0x24,
    2,
    3,
    0x01,
    0x01,
    0,
    2,
    0x03,
    0x00,
    0,
    0,

    10,
    0x24,
    0x06,
    0x31,
    0x03,
    0x01,
    0x01,
    0x02,
    0x02,
    0x00,

    9,
    0x24,
    3,
    4,

    0x02,
    0x06,
    0,
    0x31,
    0,

    9,
    4,
    AUDIO_INTERFACE + 1,
    0,
    0,
    1,
    2,
    0,
    0,

    9,
    4,
    AUDIO_INTERFACE + 1,
    1,
    1,
    1,
    2,
    0,
    0,

    7,
    0x24,
    1,
    2,
    3,
    0x01,
    0x00,

    11,
    0x24,
    2,
    1,
    2,
    2,
    16,
    1,
    LSB(44100),
    MSB(44100),
    0,

    9,
    5,
    AUDIO_TX_ENDPOINT | 0x80,
    0x09,
    LSB(AUDIO_TX_SIZE),
    MSB(AUDIO_TX_SIZE),
    1,
    0,
    0,

    7,
    0x25,
    1,
    0x00,
    0,
    0x00,
    0x00,

    9,
    4,
    AUDIO_INTERFACE + 2,
    0,
    0,
    1,
    2,
    0,
    0,

    9,
    4,
    AUDIO_INTERFACE + 2,
    1,
    2,
    1,
    2,
    0,
    0,

    7,
    0x24,
    1,
    3,
    3,
    0x01,
    0x00,

    11,
    0x24,
    2,
    1,
    2,
    2,
    16,
    1,
    LSB(44100),
    MSB(44100),
    0,

    9,
    5,
    AUDIO_RX_ENDPOINT,
    0x05,
    LSB(AUDIO_RX_SIZE),
    MSB(AUDIO_RX_SIZE),
    1,
    0,
    AUDIO_SYNC_ENDPOINT | 0x80,

    7,
    0x25,
    1,
    0x00,
    0,
    0x00,
    0x00,

    9,
    5,
    AUDIO_SYNC_ENDPOINT | 0x80,
    0x11,
    3,
    0,
    1,
    5,
    0,
#endif

#ifdef MULTITOUCH_INTERFACE

    9,
    4,
    MULTITOUCH_INTERFACE,
    0,
    1,
    0x03,
    0x00,
    0x00,
    0,

    9,
    0x21,
    0x11,
    0x01,
    0,
    1,
    0x22,
    LSB(sizeof(multitouch_report_desc)),
    MSB(sizeof(multitouch_report_desc)),

    7,
    5,
    MULTITOUCH_ENDPOINT | 0x80,
    0x03,
    MULTITOUCH_SIZE,
    0,
    1,
#endif
};

__attribute__((section(".dmabuffers"), aligned(32))) uint8_t usb_descriptor_buffer[CONFIG_DESC_SIZE];

extern struct usb_string_descriptor_struct usb_string_manufacturer_name
    __attribute__((weak, alias("usb_string_manufacturer_name_default")));
extern struct usb_string_descriptor_struct usb_string_product_name
    __attribute__((weak, alias("usb_string_product_name_default")));
extern struct usb_string_descriptor_struct usb_string_serial_number
    __attribute__((weak, alias("usb_string_serial_number_default")));

FLASHMEM const struct usb_string_descriptor_struct string0 = {4, 3, {0x0409}};

FLASHMEM const struct usb_string_descriptor_struct usb_string_manufacturer_name_default
    = {2 + MANUFACTURER_NAME_LEN * 2, 3, MANUFACTURER_NAME};

FLASHMEM const struct usb_string_descriptor_struct usb_string_product_name_default
    = {2 + PRODUCT_NAME_LEN * 2, 3, PRODUCT_NAME};

struct usb_string_descriptor_struct usb_string_serial_number_default = {12, 3, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

#ifdef MTP_INTERFACE
FLASHMEM const struct usb_string_descriptor_struct usb_string_mtp = {2 + 3 * 2, 3, {'M', 'T', 'P'}};
#endif

void usb_init_serialnumber(void)
{
  char     buf[11];
  uint32_t i, num;

  num = 1234567890U & 0xFFFFFF;

  if(num < 10000000)
    num = num * 10;
  ultoa(num, buf, 10);
  for(i = 0; i < 10; i++)
  {
    char c = buf[i];
    if(!c)
      break;
    usb_string_serial_number_default.wString[i] = c;
  }
  usb_string_serial_number_default.bLength = i * 2 + 2;
}

const usb_descriptor_list_t usb_descriptor_list[] = {

    {0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
    {0x0600, 0x0000, qualifier_descriptor, sizeof(qualifier_descriptor)},
    {0x0200, 0x0000, usb_config_descriptor_480, CONFIG_DESC_SIZE},
    {0x0700, 0x0000, usb_config_descriptor_12, CONFIG_DESC_SIZE},
#ifdef SEREMU_INTERFACE
    {0x2200, SEREMU_INTERFACE, seremu_report_desc, sizeof(seremu_report_desc)},
    {0x2100, SEREMU_INTERFACE, usb_config_descriptor_480 + SEREMU_HID_DESC_OFFSET, 9},
#endif
#ifdef KEYBOARD_INTERFACE
    {0x2200, KEYBOARD_INTERFACE, keyboard_report_desc, sizeof(keyboard_report_desc)},
    {0x2100, KEYBOARD_INTERFACE, usb_config_descriptor_480 + KEYBOARD_HID_DESC_OFFSET, 9},
#endif
#ifdef MOUSE_INTERFACE
    {0x2200, MOUSE_INTERFACE, mouse_report_desc, sizeof(mouse_report_desc)},
    {0x2100, MOUSE_INTERFACE, usb_config_descriptor_480 + MOUSE_HID_DESC_OFFSET, 9},
#endif
#ifdef JOYSTICK_INTERFACE
    {0x2200, JOYSTICK_INTERFACE, joystick_report_desc, sizeof(joystick_report_desc)},
    {0x2100, JOYSTICK_INTERFACE, usb_config_descriptor_480 + JOYSTICK_HID_DESC_OFFSET, 9},
#endif
#ifdef RAWHID_INTERFACE
    {0x2200, RAWHID_INTERFACE, rawhid_report_desc, sizeof(rawhid_report_desc)},
    {0x2100, RAWHID_INTERFACE, usb_config_descriptor_480 + RAWHID_HID_DESC_OFFSET, 9},
#endif
#ifdef FLIGHTSIM_INTERFACE
    {0x2200, FLIGHTSIM_INTERFACE, flightsim_report_desc, sizeof(flightsim_report_desc)},
    {0x2100, FLIGHTSIM_INTERFACE, usb_config_descriptor_480 + FLIGHTSIM_HID_DESC_OFFSET, 9},
#endif
#ifdef KEYMEDIA_INTERFACE
    {0x2200, KEYMEDIA_INTERFACE, keymedia_report_desc, sizeof(keymedia_report_desc)},
    {0x2100, KEYMEDIA_INTERFACE, usb_config_descriptor_480 + KEYMEDIA_HID_DESC_OFFSET, 9},
#endif
#ifdef MULTITOUCH_INTERFACE
    {0x2200, MULTITOUCH_INTERFACE, multitouch_report_desc, sizeof(multitouch_report_desc)},
    {0x2100, MULTITOUCH_INTERFACE, usb_config_descriptor_480 + MULTITOUCH_HID_DESC_OFFSET, 9},
#endif
#ifdef MTP_INTERFACE
    {0x0304, 0x0409, (const uint8_t *)&usb_string_mtp, 0},
#endif
    {0x0300, 0x0000, (const uint8_t *)&string0, 0},
    {0x0301, 0x0409, (const uint8_t *)&usb_string_manufacturer_name, 0},
    {0x0302, 0x0409, (const uint8_t *)&usb_string_product_name, 0},
    {0x0303, 0x0409, (const uint8_t *)&usb_string_serial_number, 0},
    {0, 0, NULL, 0}};

#endif
