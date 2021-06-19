#ifndef INTER_CIRCUIT_H_
#define INTER_CIRCUIT_H_

#include <fsl_gpio.h>
#include <fsl_lpi2c.h>

// #define k_I2CADDR_GPIOEX_U4 (0x24)
// #define k_I2CADDR_GPIOEX_U5 (0x27)
// #define k_I2CADDR_GPIOEX_U6 (0x20)
// #define k_I2CADDR_GPIOEX_U8 (0x22)

// TCA9555/TCA9535 (GPIO EXPANDER)
const uint8_t k_I2CADDR_GPIOEX_U4 = 0x24;
const uint8_t k_I2CADDR_GPIOEX_U5 = 0x27;
const uint8_t k_I2CADDR_GPIOEX_U6 = 0x20;
const uint8_t k_I2CADDR_GPIOEX_U8 = 0x22;

const uint8_t k_I2CREG_GPIOEX_INP0  = 0x00;
const uint8_t k_I2CREG_GPIOEX_INP1  = 0x01;
const uint8_t k_I2CREG_GPIOEX_OUTP0 = 0x02;
const uint8_t k_I2CREG_GPIOEX_OUTP1 = 0x03;
const uint8_t k_I2CREG_GPIOEX_POLP0 = 0x04;
const uint8_t k_I2CREG_GPIOEX_POLP1 = 0x05;
const uint8_t k_I2CREG_GPIOEX_CFGP0 = 0x06;
const uint8_t k_I2CREG_GPIOEX_CFGP1 = 0x07;

// #define k_I2CREG_GPIOEX_INP0  0x00
// #define k_I2CREG_GPIOEX_INP1  0x01
// #define k_I2CREG_GPIOEX_OUTP0 0x02
// #define k_I2CREG_GPIOEX_OUTP1 0x03
// #define k_I2CREG_GPIOEX_POLP0 0x04
// #define k_I2CREG_GPIOEX_POLP1 0x05
// #define k_I2CREG_GPIOEX_CFGP0 0x06
// #define k_I2CREG_GPIOEX_CFGP1 0x07

const uint8_t kNumMusicKeys     = 24;
const uint8_t k_NUM_GPIO_EX_BUF = 10; // number of devices * 2

const uint8_t kSwMusic[24][2] = {
    {1, (1U << 6)}, // 00 | PCB: SW1
    {1, (1U << 5)}, // 01 | PCB: SW2
    {1, (1U << 4)}, // 02 | PCB: SW3
    {1, (1U << 3)}, // 03 | PCB: SW4
    {1, (1U << 7)}, // 04 | PCB: SW5
    {0, (1U << 0)}, // 05 | PCB: SW6
    {1, (1U << 2)}, // 06 | PCB: SW7
    {0, (1U << 1)}, // 07 | PCB: SW8
    {1, (1U << 1)}, // 08 | PCB: SW9
    {0, (1U << 2)}, // 09 | PCB: SW10
    {1, (1U << 0)}, // 10 | PCB: SW11
    {0, (1U << 3)}, // 11 | PCB: SW12
    {0, (1U << 4)}, // 12 | PCB: SW29
    {0, (1U << 7)}, // 13 | PCB: SW30
    {0, (1U << 5)}, // 14 | PCB: SW31
    {0, (1U << 6)}, // 15 | PCB: SW32
    {5, (1U << 7)}, // 16 | PCB: SW33
    {4, (1U << 3)}, // 17 | PCB: SW34
    {4, (1U << 6)}, // 18 | PCB: SW35
    {4, (1U << 2)}, // 19 | PCB: SW36
    {4, (1U << 5)}, // 20 | PCB: SW37
    {4, (1U << 1)}, // 21 | PCB: SW38
    {4, (1U << 4)}, // 22 | PCB: SW39
    {4, (1U << 0)}  // 23 | PCB: SW40
};

const uint8_t kSwFunc[16][2] = {
    {3, (1U << 6)}, // 00
    {3, (1U << 5)}, // 01
    {3, (1U << 3)}, // 02
    {3, (1U << 4)}, // 03
    {3, (1U << 2)}, // 04
    {3, (1U << 1)}, // 05
    {3, (1U << 0)}, // 06
    {2, (1U << 7)}, // 07
    {3, (1U << 7)}, // 08
    {2, (1U << 0)}, // 09
    {2, (1U << 1)}, // 10
    {2, (1U << 2)}, // 11
    {2, (1U << 5)}, // 12
    {2, (1U << 3)}, // 13
    {2, (1U << 4)}, // 14
    {2, (1U << 6)}, // 15
};

const uint8_t kSwEnc[16][2] = {
    {5, (1U << 6)}, // 01
    {5, (1U << 5)}, // 02
    {4, (1U << 7)}, // 03
    {5, (1U << 0)}, // 04
    {5, (1U << 3)}, // 05
    {5, (1U << 4)}, // 06
    {5, (1U << 2)}, // 07
    {5, (1U << 1)}, // 08
};

const uint8_t kLeftBottomKeys[3][2] = {
    {7, (1U << 5)}, // 00 | PCB: SW1
    {7, (1U << 6)}, // 01 | PCB: SW2
    {7, (1U << 7)}, // 02 | PCB: SW3
};

const uint8_t kJoySwitch[1][2] = {{6, (1U << 0)}};

//U6  [0x20]
#define SW_MK_16 (1 << 15) // PCB: SW33
#define SW_MK_17 (1 << 3)  // PCB: SW34
#define SW_MK_18 (1 << 6)  // PCB: SW35
#define SW_MK_19 (1 << 2)  // PCB: SW36
#define SW_MK_20 (1 << 5)  // PCB: SW37
#define SW_MK_21 (1 << 1)  // PCB: SW38
#define SW_MK_22 (1 << 4)  // PCB: SW39
#define SW_MK_23 (1 << 0)  // PCB: SW40
#define ENC_0C   (1 << 14)
#define ENC_1C   (1 << 13)
#define ENC_2C   (1 << 7)
#define ENC_3C   (1 << 8)
#define ENC_4C   (1 << 11)
#define ENC_5C   (1 << 12)
#define ENC_6C   (1 << 10)
#define ENC_7C   (1 << 9)

//U8 [0x22]
#define SW_JOYSTK (1 << 0)  // P0 IN  | PCB: SW_POT1
#define TOUCH_RST (1 << 1)  // P0 OUT | PCB: NRESET_TOUCH_B
#define LED_FA_00 (1 << 2)  // P0 OUT | PCB: LED01
#define LED_FA_01 (1 << 3)  // P0 OUT | PCB: LED02
#define LED_FA_02 (1 << 4)  // P0 OUT | PCB: LED03
#define LED_FA_03 (1 << 5)  // P0 OUT | PCB: LED04
#define LED_FA_04 (1 << 6)  // P0 OUT | PCB: LED05
#define LED_FA_05 (1 << 7)  // P0 OUT | PCB: LED06
#define LED_FA_06 (1 << 8)  // P1 OUT | PCB: LED07
#define LED_FA_07 (1 << 9)  // P1 OUT | PCB: LED08
#define LED_FA_08 (1 << 10) // P1 OUT | PCB: LED09
#define LED_FA_09 (1 << 11) // P1 OUT | PCB: LED10
#define LED_FA_10 (1 << 12) // P1 OUT | PCB: LED11
#define SW_LB_00  (1 << 13) // P1 IN  | DIR: IN  | PCB: SW41
#define SW_LB_01  (1 << 14) // P1 IN  | DIR: IN  | PCB: SW42
#define SW_LB_02  (1 << 15) // P1 IN  | DIR: IN  | PCB: SW54

// Touch defines
#define ADDR_AT42QT_U9 (0x1C)
#define AT42QT_WHOAMI  0x3E

#define AT42QT_CHIP_ID         0x00
#define AT42QT_FW_VER          0x01
#define AT42QT_DET_STAT        0x02
#define AT42QT_KEY_STAT1       0x03
#define AT42QT_KEY_STAT2       0x04
#define AT42QT_SLIDER_POS      0x05
#define AT42QT_CALIB           0x06
#define AT42QT_RESET           0x07
#define AT42QT_LP              0x08
#define AT42QT_TTD             0x09
#define AT42QT_ATD             0x0A
#define AT42QT_DI              0x0B
#define AT42QT_TRD             0x0C
#define AT42QT_DHT             0x0D
#define AT42QT_SLID_OPT        0x0E
#define AT42QT_CHARGE_TIME     0x0F
#define AT42QT_KEY0_DET_THR    0x10
#define AT42QT_KEY1_DET_THR    0x11
#define AT42QT_KEY2_DET_THR    0x12
#define AT42QT_KEY3_DET_THR    0x13
#define AT42QT_KEY4_DET_THR    0x14
#define AT42QT_KEY5_DET_THR    0x15
#define AT42QT_KEY6_DET_THR    0x16
#define AT42QT_KEY7_DET_THR    0x17
#define AT42QT_KEY8_DET_THR    0x18
#define AT42QT_KEY9_DET_THR    0x19
#define AT42QT_KEY10_DET_THR   0x1A
#define AT42QT_KEY11_DET_THR   0x1B
#define AT42QT_KEY0_CTRL       0x1C
#define AT42QT_KEY1_CTRL       0x1D
#define AT42QT_KEY2_CTRL       0x1E
#define AT42QT_KEY3_CTRL       0x1F
#define AT42QT_KEY4_CTRL       0x20
#define AT42QT_KEY5_CTRL       0x21
#define AT42QT_KEY6_CTRL       0x22
#define AT42QT_KEY7_CTRL       0x23
#define AT42QT_KEY8_CTRL       0x24
#define AT42QT_KEY9_CTRL       0x25
#define AT42QT_KEY10_CTRL      0x26
#define AT42QT_KEY11_CTRL      0x27
#define AT42QT_KEY0_PULSE_SCL  0x28
#define AT42QT_KEY1_PULSE_SCL  0x29
#define AT42QT_KEY2_PULSE_SCL  0x2A
#define AT42QT_KEY3_PULSE_SCL  0x2B
#define AT42QT_KEY4_PULSE_SCL  0x2C
#define AT42QT_KEY5_PULSE_SCL  0x2D
#define AT42QT_KEY6_PULSE_SCL  0x2E
#define AT42QT_KEY7_PULSE_SCL  0x2F
#define AT42QT_KEY8_PULSE_SCL  0x30
#define AT42QT_KEY9_PULSE_SCL  0x31
#define AT42QT_KEY10_PULSE_SCL 0x32
#define AT42QT_KEY11_PULSE_SCL 0x33
#define AT42QT_KEY0_SIG_MSB    0x34
#define AT42QT_KEY0_SIG_LSB    0x35
#define AT42QT_KEY1_SIG_MSB    0x36
#define AT42QT_KEY1_SIG_LSB    0x37
#define AT42QT_KEY2_SIG_MSB    0x38
#define AT42QT_KEY2_SIG_LSB    0x39
#define AT42QT_KEY3_SIG_MSB    0x3A
#define AT42QT_KEY3_SIG_LSB    0x3B
#define AT42QT_KEY4_SIG_MSB    0x3C
#define AT42QT_KEY4_SIG_LSB    0x3D
#define AT42QT_KEY5_SIG_MSB    0x3E
#define AT42QT_KEY5_SIG_LSB    0x3F
#define AT42QT_KEY6_SIG_MSB    0x40
#define AT42QT_KEY6_SIG_LSB    0x41
#define AT42QT_KEY7_SIG_MSB    0x42
#define AT42QT_KEY7_SIG_LSB    0x43
#define AT42QT_KEY8_SIG_MSB    0x44
#define AT42QT_KEY8_SIG_LSB    0x45
#define AT42QT_KEY9_SIG_MSB    0x46
#define AT42QT_KEY9_SIG_LSB    0x47
#define AT42QT_KEY10_SIG_MSB   0x48
#define AT42QT_KEY10_SIG_LSB   0x49
#define AT42QT_KEY11_SIG_MSB   0x4A
#define AT42QT_KEY11_SIG_LSB   0x4B
#define AT42QT_REF_DATA0_MSB   0x4C
#define AT42QT_REF_DATA0_LSB   0x4D
#define AT42QT_REF_DATA1_MSB   0x4E
#define AT42QT_REF_DATA1_LSB   0x4F
#define AT42QT_REF_DATA2_MSB   0x50
#define AT42QT_REF_DATA2_LSB   0x51
#define AT42QT_REF_DATA3_MSB   0x52
#define AT42QT_REF_DATA3_LSB   0x53
#define AT42QT_REF_DATA4_MSB   0x54
#define AT42QT_REF_DATA4_LSB   0x55
#define AT42QT_REF_DATA5_MSB   0x56
#define AT42QT_REF_DATA5_LSB   0x57
#define AT42QT_REF_DATA6_MSB   0x58
#define AT42QT_REF_DATA6_LSB   0x59
#define AT42QT_REF_DATA7_MSB   0x5A
#define AT42QT_REF_DATA7_LSB   0x5B
#define AT42QT_REF_DATA8_MSB   0x5C
#define AT42QT_REF_DATA8_LSB   0x5D
#define AT42QT_REF_DATA9_MSB   0x5E
#define AT42QT_REF_DATA9_LSB   0x5F
#define AT42QT_REF_DATA10_MSB  0x60
#define AT42QT_REF_DATA10_LSB  0x61
#define AT42QT_REF_DATA11_MSB  0x62
#define AT42QT_REF_DATA11_LSB  0x63

/// Encoders
#define ADDR_STM_ENC (0x3D)

// I2C Master Device
#define LPI2C_MASTER_BASE            (LPI2C1_BASE)
#define LPI2C_CLOCK_SOURCE_SELECT    (0U) // USB1 PLL (480 MHz) as master clock
#define LPI2C_CLOCK_SOURCE_DIVIDER   (5U) // Clock divider for master
#define LPI2C_CLOCK_FREQUENCY        ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY
#define I2C_MAIN                     ((LPI2C_Type*)LPI2C_MASTER_BASE)
#define I2C_BAUDRATE                 400000U

lpi2c_master_handle_t g_i2c_m_handle;

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C"
{
#endif

  volatile bool g_i2c_completionFlag = false;
  volatile bool g_i2c_nakFlag        = false;

  static void LPI2C_Callback(LPI2C_Type* base, lpi2c_master_handle_t* handle, status_t status, void* userData)
  {
    if(status == kStatus_Success)
    {
      g_i2c_completionFlag = true;
    }
    else
    {
      PRINTF("-- [I2C] Transfer failed with status %ld \r\n", (int32_t)status);
    }

    if((status == kStatus_LPI2C_Nak))
    {
      g_i2c_nakFlag = true;
      PRINTF("-- [I2C] NAK %ld \r\n", (int32_t)status);
    }

    SDK_ISR_EXIT_BARRIER;
  }

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

void LPI2C_Init(void)
{
  lpi2c_master_config_t masterConfig;

  /*Clock setting for LPI2C*/
  CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);

  PRINTF("\r\nStart\r\n");

  /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
  LPI2C_MasterGetDefaultConfig(&masterConfig);

  /* Change the default baudrate configuration */
  masterConfig.baudRate_Hz = I2C_BAUDRATE;

  /* Initialize the LPI2C master peripheral */
  LPI2C_MasterInit(I2C_MAIN, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

  /* Create the LPI2C handle for the non-blocking transfer */
  LPI2C_MasterTransferCreateHandle(I2C_MAIN, &g_i2c_m_handle, LPI2C_Callback, NULL);
}

//? Disabled, because it needs to be rewritten
// static bool I2C_CheckGpioexTca(uint8_t dev_addr)
// {
//   lpi2c_master_transfer_t localXfer = {0};

//   uint8_t who_am_i_reg      = 0x00;
//   uint8_t who_am_i_value[1] = {0};
//   bool    find_device       = false;

//   memset(&localXfer, 0, sizeof(localXfer));

//   localXfer.slaveAddress   = dev_addr;
//   localXfer.direction      = kLPI2C_Write;
//   localXfer.subaddress     = 0x00;
//   localXfer.subaddressSize = 1U;
//   localXfer.data           = &who_am_i_reg;
//   localXfer.dataSize       = 1U;
//   localXfer.flags          = kLPI2C_TransferNoStopFlag;

//   LPI2C_MasterTransferNonBlocking(I2C_MAIN, &g_i2c_m_handle, &localXfer);

//   /*  wait for transfer completed. */
//   while((!g_i2c_nakFlag) && (!g_i2c_completionFlag)) {}

//   g_i2c_nakFlag = false;

//   if(g_i2c_completionFlag == true)
//   {
//     g_i2c_completionFlag = false;
//     find_device          = true;
//   }

//   if(find_device == true)
//   {
//     memset(&localXfer, 0, sizeof(localXfer));
//     memset(&who_am_i_value, 0, sizeof(who_am_i_value));

//     localXfer.slaveAddress   = dev_addr;
//     localXfer.direction      = kLPI2C_Read;
//     localXfer.subaddress     = 0x00;
//     localXfer.subaddressSize = 1U;
//     localXfer.data           = who_am_i_value;
//     localXfer.dataSize       = 1U;
//     localXfer.flags          = kLPI2C_TransferDefaultFlag;
//     // kLPI2C_TransferRepeatedStartFlag

//     LPI2C_MasterTransferNonBlocking(I2C_MAIN, &g_i2c_m_handle, &localXfer);

//     /*  wait for transfer completed. */
//     while((!g_i2c_nakFlag) && (!g_i2c_completionFlag)) {}

//     g_i2c_nakFlag = false;

//     if(g_i2c_completionFlag == true)
//     {
//       PRINTF("Found a device at 0x%x, WhoAmI value is 0x%x \n", dev_addr, who_am_i_value[0]);
//     }
//     else
//     {
//       PRINTF("Not a successful i2c communication for 0x%x \n", dev_addr);
//       return false;
//     }
//   }
//   else
//   {
//     PRINTF("No device found at 0x%x \n", dev_addr);
//     return false;
//   }
//   return true;
// }

static bool I2C_WriteSingleReg(uint8_t dev_addr, uint8_t reg, uint8_t val)
{
  lpi2c_master_transfer_t localXfer = {0};
  uint8_t                 regValue  = val;

  g_i2c_completionFlag = false;
  memset(&localXfer, 0, sizeof(localXfer));

  localXfer.slaveAddress   = dev_addr;
  localXfer.direction      = kLPI2C_Write;
  localXfer.subaddress     = reg;
  localXfer.subaddressSize = 1;
  localXfer.data           = &regValue;
  localXfer.dataSize       = 1;
  localXfer.flags          = kLPI2C_TransferDefaultFlag;

  LPI2C_MasterTransferNonBlocking(I2C_MAIN, &g_i2c_m_handle, &localXfer);

  /*  wait for transfer completed. */
  while((!g_i2c_nakFlag) && (!g_i2c_completionFlag)) {}

  g_i2c_nakFlag = false;

  if(g_i2c_completionFlag == true) {}

  // PRINTF("[I2C] Write reg 0x%x with val 0x%x \n", reg, regValue);

  return false;
}

static bool I2C_ReadSingleReg(uint8_t dev_addr, uint8_t reg, uint8_t* dest)
{
  lpi2c_master_transfer_t localXfer = {0};
  // uint8_t readBuffer[1];

  g_i2c_completionFlag = false;
  memset(&localXfer, 0, sizeof(localXfer));
  // memset(&readBuffer, 0, sizeof(readBuffer));

  localXfer.slaveAddress   = dev_addr;
  localXfer.direction      = kLPI2C_Read;
  localXfer.subaddress     = reg;
  localXfer.subaddressSize = 1;
  localXfer.data           = dest;
  localXfer.dataSize       = 1;
  localXfer.flags          = kLPI2C_TransferDefaultFlag;

  LPI2C_MasterTransferNonBlocking(I2C_MAIN, &g_i2c_m_handle, &localXfer);

  /*  wait for transfer completed. */
  while((!g_i2c_nakFlag) && (!g_i2c_completionFlag)) {}

  g_i2c_nakFlag = false;

  if(g_i2c_completionFlag == true)
  {
    return true;
  }

  // PRINTF("[I2C] Read reg 0x%x: val 0x%x \n", reg, readBuffer[0]);

  return false;
}

static bool I2C_ReadRegister(uint8_t dev_addr, uint8_t reg, void* dest, uint8_t size)
{
  lpi2c_master_transfer_t localXfer = {0};
  // uint8_t readBuffer[1];

  g_i2c_completionFlag = false;
  memset(&localXfer, 0, sizeof(localXfer));
  // memset(&readBuffer, 0, sizeof(readBuffer));

  localXfer.slaveAddress   = dev_addr;
  localXfer.direction      = kLPI2C_Read;
  localXfer.subaddress     = reg;
  localXfer.subaddressSize = 1;
  localXfer.data           = dest;
  localXfer.dataSize       = size;
  localXfer.flags          = kLPI2C_TransferDefaultFlag;

  LPI2C_MasterTransferNonBlocking(I2C_MAIN, &g_i2c_m_handle, &localXfer);

  /*  wait for transfer completed. */
  while((!g_i2c_nakFlag) && (!g_i2c_completionFlag)) {}

  g_i2c_nakFlag = false;

  if(g_i2c_completionFlag == true)
  {
    return true;
  }

  return false;

  // PRINTF("[I2C] Read reg 0x%x: val 0x%x \n", reg, readBuffer[0]);
}

void I2C_AT42QT_Init(void)
{
  // * 8. Low power mode
  // This 8-bit val determines the number of 16ms intervals between
  // key measurements. Longer – lower power consumption at the
  // expense of a slower response to touch.
  // 0x00 - Power Down
  // 1 - 16ms; 2 - 32ms; 3 - 48ms; 255 - 4.08s
  // Default 1
  // I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_LP, 1);

  // * 9. Toward Touch Drift (TTD)
  // Default 20; Range 0-127
  // I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_TTD, 20);

  // * 10. Away From Touch Drift (ATD)
  // Default 5; Range 0-127
  // I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_ATD, 5);

  // * 11. Detection Integrator (DI)
  // Default 4; Range 0-31
  // I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_DI, 4);

  // * 12. Touch Recall Delay (TRD)
  // Default 255; Range 0-255
  // I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_TRD, 255);

  // * 13. Drift Hold Time (DHT)
  // Default 25; Range 0-255
  // I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_DHT, 25);

  // * 14. Slider Options
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_SLID_OPT, 0x80);

  // * 15. Charge Time
  // Prolongs the charge-transfer period of signal acquisition
  // by 1 μs per count. Allows full charge-transfer for keys with
  // heavy Rs / Cx loading.
  // Default 0; Range 0-255
  // I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_CHARGE_TIME, 0);

  // * 16-27. Detect Threshold (DTHR)
  // Default 10; Range 1-255
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY0_DET_THR, 10);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY1_DET_THR, 10);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY2_DET_THR, 10);

  // 28-39. Key Control
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY3_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY4_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY5_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY6_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY7_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY8_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY9_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY10_CTRL, 1);
  I2C_WriteSingleReg(ADDR_AT42QT_U9, AT42QT_KEY11_CTRL, 1);

  // uint8_t readd[1]      = {0};
  // uint8_t init_data0[1] = {0xFF};
  // uint8_t init_data1[8] = {
  //     6,    //REG_AT42QT_LP   //[GOOD] default 1
  //     20,   //REG_AT42QT_TTD // Default
  //     5,    //REG_AT42QT_ATD // Default
  //     4,    //REG_AT42QT_DI   // DEFAULT IS 4 [GOOD]
  //     200,  //REG_AT42QT_TRD  // Default 255
  //     200,  //REG_AT42QT_DHT  // Default 25
  //     0x80, //REG_AT42QT_SLID_OPT
  //     0,    //REG_AT42QT_CHARGE_TIME   //DEFAULT IS ZERO
  // };
}

void SetLedBar(uint8_t val, uint8_t mode)
{
  uint8_t  num1, num2;
  uint16_t accum = 0x0;

  if(val < 0 || val > 11)
    return;

  if(mode == 0)
    val = 11 - val;

  if(val == 11)
  {
    accum = 0x0;
  }
  else if(val == 0)
  {
    accum = 0x7FF;
  }
  else
  {
    for(uint8_t i = 0; i < 12; i++)
    {
      if(i >= val)
        accum = (accum | (1 << (i)));
    }
  }

  num1 = (uint8_t)((accum << 2) | 0x02);
  num2 = (uint8_t)(accum >> 6);

  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP0, num1);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP1, num2);
}

// void SetLedBar(uint8_t val)
// {
//   if(val < 0 || val > 10)
//     return;

//   uint8_t  num1, num2;
//   uint16_t accum = 0;

//   for(uint8_t i = 0; i < 11; i++)
//   {
//     if(i <= val){
//       accum = (accum | (1 << i))
//     }
//   }

//   // for(uint8_t i = 0; i < 11; i++)
//   // {
//   //   if(i < 6)
//   //   {
//   //     num1 = ((val ? 0x1F : 0) << 2) | 0x02;
//   //     // 0x02
//   //   }
//   //   else
//   //   {
//   //   }
//   // }

//   I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP0, num1);
//   I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP1, 0x00);

// P0: LED5 | LED4 | LED3 | LED2 | LED1 | LED0 | TRST | JOYS
// I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP0, 0x06);
// P1: SWL2 | SWL1 | SWL0 | LE10 | LED9 | LED8 | LED7 | LED6
// I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP1, 0x00);
// }

void LPUART_SetupAllSecondaries(void)
{
  uint8_t readBuffer[1] = {0};

  // I2C_CheckGpioexTca(k_I2CADDR_GPIOEX_U4);
  // I2C_CheckGpioexTca(k_I2CADDR_GPIOEX_U5);
  // I2C_CheckGpioexTca(k_I2CADDR_GPIOEX_U6);
  // I2C_CheckGpioexTca(k_I2CADDR_GPIOEX_U8);
  // I2C_CheckGpioexTca(ADDR_STM_ENC);

  // U4
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U4, k_I2CREG_GPIOEX_CFGP0, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U4, k_I2CREG_GPIOEX_POLP0, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U4, k_I2CREG_GPIOEX_CFGP1, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U4, k_I2CREG_GPIOEX_POLP1, 0xFF);

  // U5
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U5, k_I2CREG_GPIOEX_CFGP0, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U5, k_I2CREG_GPIOEX_POLP0, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U5, k_I2CREG_GPIOEX_CFGP1, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U5, k_I2CREG_GPIOEX_POLP1, 0xFF);

  // U6
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U6, k_I2CREG_GPIOEX_CFGP0, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U6, k_I2CREG_GPIOEX_POLP0, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U6, k_I2CREG_GPIOEX_CFGP1, 0xFF);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U6, k_I2CREG_GPIOEX_POLP1, 0xFF);

  // U8
  // P0: LED5 | LED4 | LED3 | LED2 | LED1 | LED0 | TRST | JOYS
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_CFGP0, 0x01);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_POLP0, 0x01);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP0, 0x02);
  // P1: SWL2 | SWL1 | SWL0 | LE10 | LED9 | LED8 | LED7 | LED6
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_CFGP1, 0xE0);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_POLP1, 0xE0);
  I2C_WriteSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_OUTP1, 0x00);

  // << 2

  // Read all GPIO Expanders
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U4, k_I2CREG_GPIOEX_INP0, readBuffer);
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U4, k_I2CREG_GPIOEX_INP1, readBuffer);
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U5, k_I2CREG_GPIOEX_INP0, readBuffer);
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U5, k_I2CREG_GPIOEX_INP1, readBuffer);
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U6, k_I2CREG_GPIOEX_INP0, readBuffer);
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U6, k_I2CREG_GPIOEX_INP1, readBuffer);
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_INP0, readBuffer);
  I2C_ReadSingleReg(k_I2CADDR_GPIOEX_U8, k_I2CREG_GPIOEX_INP1, readBuffer);

  // I2C_CheckGpioexTca(ADDR_AT42QT_U9);
  I2C_ReadSingleReg(ADDR_AT42QT_U9, 0x00, readBuffer);
  I2C_ReadSingleReg(ADDR_AT42QT_U9, 0x01, readBuffer);

  I2C_AT42QT_Init();
  // Read DETECTION STATUS
  I2C_ReadSingleReg(ADDR_AT42QT_U9, AT42QT_DET_STAT, readBuffer);
  // Read Keys
  I2C_ReadSingleReg(ADDR_AT42QT_U9, AT42QT_KEY_STAT1, readBuffer);
  I2C_ReadSingleReg(ADDR_AT42QT_U9, AT42QT_KEY_STAT2, readBuffer);
  // Read Slider
  I2C_ReadSingleReg(ADDR_AT42QT_U9, AT42QT_SLIDER_POS, readBuffer);

  // // Scan I2C
  // PRINTF("Scanning I2C domain...\n");
  // for (uint8_t i = 0; i < 127; i++) {
  //   I2C_CheckGpioexTca(i);
  // }
}

#endif /* INTER_CIRCUIT_H_ */
