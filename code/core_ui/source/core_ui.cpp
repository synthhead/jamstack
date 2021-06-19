#include <stdio.h>
#include <math.h>

#include "fsl_adc.h"
#include "fsl_lpi2c.h"
#include "fsl_lpspi.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "inter_circuit.h"
#include "uart_queue.h"
#include "minigfx.h"

#define CORE_UI_EXT_LPUART          LPUART1
#define CORE_UI_EXT_LPUART_CLK_FREQ BOARD_DebugConsoleSrcFreq()

/// ADC Defs
#define JOY_ADC_BASE    ADC1
#define JOY_ADC_PIN_X   11U
#define JOY_ADC_PIN_Y   10U
#define JOY_ADC_CHGROUP 0U

lpuart_handle_t g_lpuartHandle;
volatile bool   uart_rxBufferEmpty = true;
volatile bool   uart_txBufferFull  = false;
volatile bool   uart_txOnGoing     = false;
volatile bool   uart_rxOnGoing     = false;

coreui::UartQueue<16> uart_queue;

volatile uint32_t g_systickCounter = 20U;

uint8_t oled_buf[256 * 64 / 2];

struct UiState
{
  uint8_t musicKeys[kNumMusicKeys];
  uint8_t funcKeys[16];
  uint8_t leftbottomKeys[3];
  uint8_t encDials[8];
  uint8_t encButtons[8];
  uint8_t tsliderValues[1];
  uint8_t tsliderDetection;
  uint8_t joyXY[2];
  uint8_t joySwitch;
};

UiState ui_state;

volatile uint8_t g_screen_id = 0;

volatile bool g_i2c_int_flag        = false;
volatile bool g_i2c_int_repeat_flag = false;

// Global IRQ handlers MUST use
// C linkage
#ifdef __cplusplus
extern "C"
{
#endif

  void SysTick_Handler(void)
  {
    if(g_systickCounter != 0U)
      g_systickCounter--;
  }

  void GPIO1_Combined_0_15_IRQHandler(void)
  {
    // __disable_irq();
    GPIO_PortClearInterruptFlags(GPIO1, 1U << 0U);
    g_i2c_int_flag = true;
    // __enable_irq();
    SDK_ISR_EXIT_BARRIER;
  }

  void LPUART_UserCallback(LPUART_Type* base, lpuart_handle_t* handle, status_t status, void* userData)
  {
    userData = userData;

    if(kStatus_LPUART_TxIdle == status)
    {
      uart_txBufferFull = false;
      uart_txOnGoing    = false;
    }

    if(kStatus_LPUART_RxIdle == status)
    {
      uart_rxBufferEmpty = false;
      uart_rxOnGoing     = false;
    }
  }

#ifdef __cplusplus
}
#endif

float my_map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min) + 0.5f;
}

int32_t divRoundClosest(const int32_t n, const int32_t d)
{
  return ((n < 0) ^ (d < 0)) ? ((n - d / 2) / d) : ((n + d / 2) / d);
}

// void UpdateUiStateItemBool()
// {
//   uint8_t val = gpioexBuf[kSwFunc[i][0]] & kSwFunc[i][1];
//   if(val != ui_state.funcKeys[i])
//   {
//     ui_state.funcKeys[i] = val;
//     uart_queue.AddEntry(i + 40, val ? 1 : 0);
//   }
// }

void updateEncoderDials(uint8_t id, int8_t val)
{
  static uint8_t lastEncDials[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int8_t         value           = ui_state.encDials[id];
  value += val;

  if(value < 0)
    value = 0;
  else if(value > 63)
    value = 63;

  if(val)
    uart_queue.AddEntry(id + 30, (uint8_t)val + 64);

  if(lastEncDials[id] != value)
  {
    ui_state.encDials[id] = value;
    lastEncDials[id]      = value;
    // PRINTF("Changed %i\r\n", id);
  }
}

int8_t CheckAllI2cDevices(void)
{
  uint8_t gpioexBuf[k_NUM_GPIO_EX_BUF];
  uint8_t touchBuf[2] = {0, 0}; // { <status>, <value> }
  int8_t  encBuf[8]   = {0, 0, 0, 0, 0, 0, 0, 0};
  // uint8_t encStatus   = 0;

  I2C_ReadRegister(k_I2CADDR_GPIOEX_U4, k_I2CREG_GPIOEX_INP0, gpioexBuf + 0, 2);
  I2C_ReadRegister(k_I2CADDR_GPIOEX_U5, k_I2CREG_GPIOEX_INP0, gpioexBuf + 2, 2);
  I2C_ReadRegister(k_I2CADDR_GPIOEX_U6, k_I2CREG_GPIOEX_INP0, gpioexBuf + 4, 2);
  I2C_ReadRegister(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_INP0, gpioexBuf + 6, 2);

  I2C_ReadSingleReg(ADDR_AT42QT_U9, AT42QT_DET_STAT, touchBuf + 0);
  I2C_ReadSingleReg(ADDR_AT42QT_U9, AT42QT_SLIDER_POS, touchBuf + 1);

  // I2C_ReadRegister(ADDR_STM_ENC, 0x02, &encStatus, 1);
  I2C_ReadRegister(ADDR_STM_ENC, 0x03, (uint8_t*)(encBuf + 0), 8);

  for(uint8_t i = 0; i < kNumMusicKeys; i++)
  {
    uint8_t val = gpioexBuf[kSwMusic[i][0]] & kSwMusic[i][1];
    if(val != ui_state.musicKeys[i])
    {
      ui_state.musicKeys[i] = val;
      uart_queue.AddEntry(i, val ? 1 : 0); // MK: +0
    }
  }

  for(uint8_t i = 0; i < 8; i++)
  {
    updateEncoderDials(i, encBuf[i]); // ENC: +30
  }

  for(uint8_t i = 0; i < 16; i++)
  {
    uint8_t val = gpioexBuf[kSwFunc[i][0]] & kSwFunc[i][1];
    if(val != ui_state.funcKeys[i])
    {
      ui_state.funcKeys[i] = val;
      uart_queue.AddEntry(i + 40, val ? 1 : 0); // FUNC: +40
    }
  }

  for(uint8_t i = 0; i < 3; i++)
  {
    uint8_t val = gpioexBuf[kLeftBottomKeys[i][0]] & kLeftBottomKeys[i][1];
    if(val != ui_state.leftbottomKeys[i])
    {
      ui_state.leftbottomKeys[i] = val;
      uart_queue.AddEntry(i + 56, val ? 1 : 0); // LBKEYS: +56
    }
  }

  for(uint8_t i = 0; i < 8; i++)
  {
    uint8_t val = gpioexBuf[kSwEnc[i][0]] & kSwEnc[i][1];
    if(val != ui_state.encButtons[i])
    {
      ui_state.encButtons[i] = val;
      uart_queue.AddEntry(i + 60, val ? 1 : 0); // ENC_BTN: +60
    }
  }

  if(ui_state.tsliderDetection != (touchBuf[0] & 0x1U))
  {
    ui_state.tsliderDetection = (touchBuf[0] & 0x1U);
    uart_queue.AddEntry(69, touchBuf[0]); // TOUCH STRIP: 70
  }

  if(ui_state.tsliderValues[0] != touchBuf[1])
  {
    ui_state.tsliderValues[0] = touchBuf[1];
    uart_queue.AddEntry(70, touchBuf[1]); // TOUCH STRIP: 70
  }

  static uint8_t last_joyswitch_temp_value = 0;

  uint8_t joyswitch_temp_value = gpioexBuf[kJoySwitch[0][0]] & kJoySwitch[0][1];
  if(ui_state.joySwitch != joyswitch_temp_value)
  {
    ui_state.joySwitch = joyswitch_temp_value;
    uart_queue.AddEntry(71, joyswitch_temp_value); // JOY SWITCH: 71
    if(ui_state.joySwitch == 1)
    {
      g_screen_id++;

      if(g_screen_id > 3)
      {
        g_screen_id = 0;
      }
    }
  }

  // SetLedBar(static_cast<uint8_t>(my_map(touchBuf[1], 0, 255, 0, 11)), 0);
  SetLedBar(divRoundClosest(touchBuf[1] - 2, 23), 0);

  // if(gpioexBuf[8] != 0)
  //   return 16;
  // else
  return 0;
}

int main(void)
{
  // PRINTF("CORE_UI: Startup \r\n");
  // PRINTF("CORE_UI: Startup \r\n");

  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
  BOARD_InitDebugConsole();
#endif

  // UART
  lpuart_config_t config;
  /*
   * config.baudRate_Bps = 115200U;
   * config.parityMode = kLPUART_ParityDisabled;
   * config.stopBitCount = kLPUART_OneStopBit;
   * config.txFifoWatermark = 0;
   * config.rxFifoWatermark = 0;
   * config.enableTx = false;
   * config.enableRx = false;
   */
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
  config.enableTx     = true;
  config.enableRx     = true;
  LPUART_Init(CORE_UI_EXT_LPUART, &config, CORE_UI_EXT_LPUART_CLK_FREQ);
  LPUART_TransferCreateHandle(CORE_UI_EXT_LPUART, &g_lpuartHandle, LPUART_UserCallback, NULL);

  //   // Other
  LPSPI_Init();
  init_ssd1322();
  LPI2C_Init();
  EnableIRQ(GPIO1_Combined_0_15_IRQn);

  uart_queue.Init();

  uint8_t temp_val = GPIO_PinRead(GPIO1, 0U);
  PRINTF("INT pin is %u ", temp_val);

  // ADC
  adc_config_t         adcConfigStrcut;
  adc_channel_config_t adcChannelConfigStruct;
  adc_offest_config_t  adcOffsetStruct;
  adcOffsetStruct.enableSigned = true;
  adcOffsetStruct.offsetValue  = 200;

  ADC_GetDefaultConfig(&adcConfigStrcut);
  adcConfigStrcut.resolution = kADC_Resolution8Bit;
  ADC_Init(JOY_ADC_BASE, &adcConfigStrcut);

  /* Do auto hardware calibration. */
  if(kStatus_Success == ADC_DoAutoCalibration(JOY_ADC_BASE))
  {
    PRINTF("ADC_DoAutoCalibration() Done.\r\n");
  }
  else
  {
    PRINTF("ADC_DoAutoCalibration() Failed.\r\n");
  }

  adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;
  // PRINTF("ADC Full Range: %d\r\n", g_Adc_12bitFullRange);

  ///////////////////////////////
  LPUART_SetupAllSecondaries();
  CheckAllI2cDevices();

  PRINTF("Setup done\n");

  // memset(ui_state, 0, sizeof(UiState));
  memset(ui_state.musicKeys, 0, kNumMusicKeys);
  memset(ui_state.funcKeys, 0, 16);
  memset(ui_state.encButtons, 0, 8);
  memset(ui_state.encDials, 0, 8);

  while(1)
  {
    if(uart_queue.size())
    {
      while(uart_queue.size() && !uart_txOnGoing)
      {
        coreui::QueueEntry entry = uart_queue.most_recent_entry();

        uint8_t tag;
        uint8_t tag_offset = 0;

        // CC
        if(entry.item >= 30)
        {
          tag        = 13;
          tag_offset = -30;
        }
        // NoteOff, only when it's LOWER than 30 because `else if`
        else if(!entry.state)
        {
          tag = 12;
        }
        // NoteOn
        else
        {
          tag = 11;
        }

        uint8_t noteEvent[4] = {tag, entry.item + tag_offset, entry.state, 0};

        lpuart_transfer_t xfer;
        xfer.data      = noteEvent;
        xfer.dataSize  = 4U;
        uart_txOnGoing = true;
        LPUART_TransferSendNonBlocking(CORE_UI_EXT_LPUART, &g_lpuartHandle, &xfer);
        uart_queue.RemoveEntry(entry.item);
        // PRINTF("entry: %i, state: %i\r\n", entry.item, entry.state);
      }
    }

    fillBuffer(oled_buf, 0);

    if(g_screen_id == 0)
    {
      drawTestUiScreen(oled_buf,
                       ui_state.funcKeys,
                       ui_state.encButtons,
                       ui_state.musicKeys,
                       (uint8_t*)ui_state.encDials,
                       ui_state.tsliderValues,
                       ui_state.joyXY,
                       ui_state.joySwitch,
                       ui_state.leftbottomKeys);
    }
    else if(g_screen_id == 1)
    {
      drawTapeScreen(oled_buf);
    }
    else if(g_screen_id == 2)
    {
      drawSinRecScreen(oled_buf);
    }
    else if(g_screen_id == 4)
    {
      drawGradients(oled_buf);
    }

    send_buffer_to_OLED(oled_buf, 0, 0);

    uint8_t int_pin_value = !GPIO_PinRead(BOARD_INITPINS_INT_I2C1_PERIPHERAL, BOARD_INITPINS_INT_I2C1_CHANNEL);

    // I2C int just happened
    if(g_i2c_int_flag || g_i2c_int_repeat_flag || int_pin_value)
    {
      int8_t i2c_status = CheckAllI2cDevices();

      if(i2c_status == 16)
        g_i2c_int_repeat_flag = true;
      else
        g_i2c_int_repeat_flag = false;

      __disable_irq();
      g_i2c_int_flag = false;
      __enable_irq();
    }

    uint8_t adc_temp_val = 0;

    adcChannelConfigStruct.channelNumber = JOY_ADC_PIN_X;
    adcOffsetStruct.offsetValue          = 0;
    ADC_SetOffsetConfig(JOY_ADC_BASE, &adcOffsetStruct);
    ADC_SetChannelConfig(JOY_ADC_BASE, JOY_ADC_CHGROUP, &adcChannelConfigStruct);
    while(0U == ADC_GetChannelStatusFlags(JOY_ADC_BASE, JOY_ADC_CHGROUP)) {}
    adc_temp_val = ADC_GetChannelConversionValue(JOY_ADC_BASE, JOY_ADC_CHGROUP);
    if(ui_state.joyXY[0] != adc_temp_val)
    {
      ui_state.joyXY[0] = adc_temp_val;
      // uart_queue.AddEntry(72, adc_temp_val); // JOY X: 72
    }

    adcChannelConfigStruct.channelNumber = JOY_ADC_PIN_Y;
    adcOffsetStruct.offsetValue          = 245;
    ADC_SetOffsetConfig(JOY_ADC_BASE, &adcOffsetStruct);
    ADC_SetChannelConfig(JOY_ADC_BASE, JOY_ADC_CHGROUP, &adcChannelConfigStruct);
    while(0U == ADC_GetChannelStatusFlags(JOY_ADC_BASE, JOY_ADC_CHGROUP)) {}
    adc_temp_val = ADC_GetChannelConversionValue(JOY_ADC_BASE, JOY_ADC_CHGROUP);
    if(ui_state.joyXY[1] != adc_temp_val)
    {
      ui_state.joyXY[1] = adc_temp_val;
      // uart_queue.AddEntry(73, adc_temp_val); // JOY Y: 72
    }
  }
}
