#define __IMXRT__

#include "MIMXRT1021.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "codec.h"
#include "thing_uart.h"

#include "Jamstack.h"
#include "Shez.h"
#include "central.h"

#include "fstyle_usb.h"
#include "helpers.h"
#include "sdram.h"

#include "multi_bandlim.h"
#include "multi_keturi.h"
#include "samples/SoundSampleHihat.h"

// when we'll move out to separate init file,
// the following lines will be gone
extern "C" void usb_init(void);
extern "C" void usb_init_alt(void);

const bool gFeatureShutdownManagement = false;

volatile uint32_t clock_test_count   = 0;
volatile bool     gShutdownRequested = false;

uint32_t g_audio_clock_updates = 0;

//? This part was in systicker.cpp
extern volatile uint32_t systick_millis_count;
extern volatile uint32_t systick_cycle_count;

extern "C" void SysTick_Handler(void)
{
  systick_cycle_count = ARM_DWT_CYCCNT;
  systick_millis_count++;
}

extern "C" void SNVS_LP_WRAPPER_IRQHandler(void)
{
  if(SNVS_HPSR & 0x40)
  {
    SNVS_HPCOMR |= (1 << 31); //| (1 << 4);
    SNVS_LPSR |= (1 << 18) | (1 << 17);

    PRINTF("SNVS ISR \r\n");
    gShutdownRequested = true;
  }
  else
  {
    PRINTF("ERR! SNVS ISR ELSE \r\n");
    while(1) {}
  }
}

void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  core_sound::central.NoteOn(0U, sendXfer.data[1] + 36, 127U);
}

void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  core_sound::central.NoteOff(0U, sendXfer.data[1] + 36, 0U);
}

void onControlChange(uint8_t channel, uint8_t control, uint8_t data)
{
  core_sound::central.ControlChange(0U, sendXfer.data[1] + 34, sendXfer.data[2]);
}

void onSysExChunk(const uint8_t *data, uint16_t length, bool last)
{
  PRINTF("Sysex OK\n");
}

void usb_alt_start(void)
{
  USB_DeviceClockInit();
}

int main(void)
{
  BOARD_InitBootPins();
  BOARD_InitSDRAMPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();
  BOARD_InitDebugConsole();

  // // SEMC Clocks
  // CLOCK_InitSysPfd(kCLOCK_Pfd2, 29);
  // /* Set semc clock to 163.86 MHz */
  // CLOCK_SetMux(kCLOCK_SemcMux, 1);
  // CLOCK_SetDiv(kCLOCK_SemcDiv, 1);

  // // Init SEMC
  // PRINTF("Init SEMC; Clock speed: %d\n", EXAMPLE_SEMC_CLK_FREQ);
  // if(BOARD_InitSEMC() != kStatus_Success)
  // {
  //   PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
  // }

  systick_init();
  usb_pll_start();
  // usb_alt_start();
  usb_init();
  // usb_init_alt();
  i2c_begin();
  uart_init();
  codec_init();

  if(gFeatureShutdownManagement)
  {
    SNVS_LPSR |= (1 << 18) | (1 << 17);
    __NVIC_ClearPendingIRQ(SNVS_LP_HP_WRAPPER_IRQn);
    __NVIC_SetPriority(SNVS_LP_HP_WRAPPER_IRQn, 255); // lowest
    // memory barrier
    asm volatile("dsb");
    EnableIRQ(SNVS_LP_HP_WRAPPER_IRQn);
  }

  SoundRam(40);

  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleNoteOff(onNoteOff);
  usbMIDI.setHandleControlChange(onControlChange);
  usbMIDI.setHandleSystemExclusive(onSysExChunk);

  core_sound::central.Init();

  SoundCpuUsageMaxReset();
  SoundRamUsageMaxReset();

  uint32_t hz1_timer_num      = 0;
  uint32_t hz100_timer_num    = 0;
  uint16_t midi_reads_per_sec = 0;

  PRINTF("Setup done %d %f\r\n", 1, 2.345678);

  while(1)
  {
    if(millis() >= hz100_timer_num)
    {
      usbMIDI.read();
      midi_reads_per_sec++;

      hz100_timer_num = millis() + 99;
    }

    do_uart_thing();
    jamstack::tempo.ProcessInternalClockEvents();
    testseq.UpdateState();

    if(millis() >= hz1_timer_num)
    {
      PRINTF("Proc: %i (%i); Mem: %u (%u) \r\n", //
             static_cast<int>(SoundCpuUsage()),
             static_cast<int>(SoundCpuUsageMax()),
             SoundRamUsage(),
             SoundRamUsageMax());

      // PRINTF("Bang %u, midi_reads_per_sec: %u, clock_test_count: %u, g_audio_clock_updates: %u  \r\n", //
      //        hz1_timer_num,
      //        midi_reads_per_sec,
      //        clock_test_count,
      //        g_audio_clock_updates);

      midi_reads_per_sec = 0;
      clock_test_count   = 0;

      hz1_timer_num         = millis() + 999; // 1hz (1000) - 1, because we're loosing 1 ms somewhere
      g_audio_clock_updates = 0;
    }

    if(gShutdownRequested)
    {
      SNVS_LPSR |= (1 << 18) | (1 << 17);
      __disable_irq();
      __NVIC_ClearPendingIRQ(SNVS_LP_HP_WRAPPER_IRQn);

      PRINTF("Should Power off now\r\n");
      gShutdownRequested = false;

      // Power off
      SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
      asm volatile("dsb");
      while(1)
      {
        asm("wfi");
      }
    }
  } // Loop end
}
