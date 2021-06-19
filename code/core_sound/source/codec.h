#ifndef CODEC_H_
#define CODEC_H_

#include "board.h"
#include "fsl_lpi2c.h"
#include "pin_mux.h"

// Page select register
#define AIC3X_PAGE_SELECT 0
// Software reset register
#define AIC3X_RESET 1

// Codec Sample rate select register
#define AIC3X_SAMPLE_RATE_SEL_REG 2
// PLL progrramming register A
#define AIC3X_PLL_PROGA_REG 3
// PLL progrramming register B
#define AIC3X_PLL_PROGB_REG 4
// PLL progrramming register C
#define AIC3X_PLL_PROGC_REG 5
// PLL progrramming register D
#define AIC3X_PLL_PROGD_REG 6
// Codec datapath setup register
#define AIC3X_CODEC_DATAPATH_REG 7
// Audio serial data interface control register A
#define AIC3X_ASD_INTF_CTRLA 8
// Audio serial data interface control register B
#define AIC3X_ASD_INTF_CTRLB 9
// Audio serial data interface control register C
#define AIC3X_ASD_INTF_CTRLC 10
// Audio overflow status and PLL R value programming register
#define AIC3X_OVRF_STATUS_AND_PLLR_REG 11
// Audio codec digital filter control register
#define AIC3X_CODEC_DFILT_CTRL 12
// Headset/button press detection register
#define AIC3X_HEADSET_DETECT_CTRL_A 13
#define AIC3X_HEADSET_DETECT_CTRL_B 14
// ADC PGA Gain control registers
#define LADC_VOL 15
#define RADC_VOL 16
// MIC3 control registers
#define MIC3LR_2_LADC_CTRL 17
#define MIC3LR_2_RADC_CTRL 18
// Line1 Input control registers
#define LINE1L_2_LADC_CTRL 19
#define LINE1R_2_LADC_CTRL 21
#define LINE1R_2_RADC_CTRL 22
#define LINE1L_2_RADC_CTRL 24
// Line2 Input control registers
#define LINE2L_2_LADC_CTRL 20 // RESERVED!
#define LINE2R_2_RADC_CTRL 23 // RESERVED!
// MICBIAS Control Register
#define MICBIAS_CTRL 25

// AGC Control Registers A, B, C
#define LAGC_CTRL_A 26
#define LAGC_CTRL_B 27
#define LAGC_CTRL_C 28
#define RAGC_CTRL_A 29
#define RAGC_CTRL_B 30
#define RAGC_CTRL_C 31

// DAC Power and Left High Power Output control registers
#define DAC_PWR 37
#define HPLCOM_CFG 37
// Right High Power Output control registers
#define HPRCOM_CFG 38
#define CODEC_HP_OUT_STAGE_CTL 40
// DAC Output Switching control registers
#define DAC_LINE_MUX 41
// High Power Output Driver Pop Reduction registers
#define HPOUT_POP_REDUCTION 42
// DAC Digital control registers
#define LDAC_VOL 43
#define RDAC_VOL 44
// Left High Power Output control registers
#define LINE2L_2_HPLOUT_VOL 45
#define PGAL_2_HPLOUT_VOL 46
#define DACL1_2_HPLOUT_VOL 47
#define LINE2R_2_HPLOUT_VOL 48
#define PGAR_2_HPLOUT_VOL 49
#define DACR1_2_HPLOUT_VOL 50
#define HPLOUT_CTRL 51
// Left High Power COM control registers
#define LINE2L_2_HPLCOM_VOL 52
#define PGAL_2_HPLCOM_VOL 53
#define DACL1_2_HPLCOM_VOL 54
#define LINE2R_2_HPLCOM_VOL 55
#define PGAR_2_HPLCOM_VOL 56
#define DACR1_2_HPLCOM_VOL 57
#define HPLCOM_CTRL 58
// Right High Power Output control registers
#define LINE2L_2_HPROUT_VOL 59
#define PGAL_2_HPROUT_VOL 60
#define DACL1_2_HPROUT_VOL 61
#define LINE2R_2_HPROUT_VOL 62
#define PGAR_2_HPROUT_VOL 63
#define DACR1_2_HPROUT_VOL 64
#define HPROUT_CTRL 65
// Right High Power COM control registers
#define LINE2L_2_HPRCOM_VOL 66
#define PGAL_2_HPRCOM_VOL 67
#define DACL1_2_HPRCOM_VOL 68
#define LINE2R_2_HPRCOM_VOL 69
#define PGAR_2_HPRCOM_VOL 70
#define DACR1_2_HPRCOM_VOL 71
#define HPRCOM_CTRL 72
// Mono Line Output Plus/Minus control registers
#define LINE2L_2_MONOLOPM_VOL 73
#define PGAL_2_MONOLOPM_VOL 74
#define DACL1_2_MONOLOPM_VOL 75
#define LINE2R_2_MONOLOPM_VOL 76
#define PGAR_2_MONOLOPM_VOL 77
#define DACR1_2_MONOLOPM_VOL 78
#define MONOLOPM_CTRL 79
// Left Line Output Plus/Minus control registers
#define LINE2L_2_LLOPM_VOL 80
#define PGAL_2_LLOPM_VOL 81
#define DACL1_2_LLOPM_VOL 82
#define LINE2R_2_LLOPM_VOL 83
#define PGAR_2_LLOPM_VOL 84
#define DACR1_2_LLOPM_VOL 85
#define LLOPM_CTRL 86
// Right Line Output Plus/Minus control registers
#define LINE2L_2_RLOPM_VOL 87
#define PGAL_2_RLOPM_VOL 88
#define DACL1_2_RLOPM_VOL 89
#define LINE2R_2_RLOPM_VOL 90
#define PGAR_2_RLOPM_VOL 91
#define DACR1_2_RLOPM_VOL 92
#define RLOPM_CTRL 93
// Clocks
#define AIC3X_CLOCK_REG 101
#define AIC3X_CLKGEN_CTRL_REG 102

#define CODEC_BYPASS_SEL_PWR_DN 108
#define CODEC_DAC_QUIESCENT 109

// Route bits
#define ROUTE_ON 0x80

// Mute bits
#define UNMUTE 0x08
#define MUTE_ON 0x80

#define INVERT_VOL(val) (0x7f - val)

// Default output volume (inverted)
#define DEFAULT_VOL INVERT_VOL(0x50)

// Default input volume
#define DEFAULT_GAIN 0x30

//
//
//

#define EXAMPLE_I2C_MASTER_BASE (LPI2C1_BASE)
#define I2C_MASTER_SLAVE_ADDR_7BIT 0x18U
#define I2C_BAUDRATE 100000U

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY

#define EXAMPLE_I2C_MASTER ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)

/* I2C instance and clock */
#define DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

lpi2c_master_handle_t   g_m_handle;
volatile bool           g_MasterCompletionFlag = false;
lpi2c_master_transfer_t i2c_transfer           = {0};

static void lpi2c_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
  /* Signal transfer success when received success status. */
  if(status == kStatus_Success)
  {
    g_MasterCompletionFlag = true;
  }
}

void i2c_begin()
{
  /*Clock setting for LPI2C*/
  CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);

  lpi2c_master_config_t masterConfig;

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
  LPI2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

  /* Create the LPI2C handle for the non-blocking transfer */
  LPI2C_MasterTransferCreateHandle(EXAMPLE_I2C_MASTER, &g_m_handle, lpi2c_master_callback, NULL);

  //   uint8_t deviceAddress = 0x01U;
  i2c_transfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
  i2c_transfer.direction    = kLPI2C_Write;
  //   i2c_transfer.subaddress = (uint32_t)deviceAddress;
  i2c_transfer.subaddressSize = 1U;
  // i2c_transfer.data = g_master_txBuff;
  i2c_transfer.dataSize = 1U;
  i2c_transfer.flags    = kLPI2C_TransferDefaultFlag;
}

int i2c_write(uint8_t reg_address, uint8_t reg_value)
{
  status_t reVal = kStatus_Fail;

  // set reg_address
  i2c_transfer.direction      = kLPI2C_Write;
  i2c_transfer.subaddress     = reg_address;
  i2c_transfer.subaddressSize = 1U;
  i2c_transfer.data           = &reg_value;
  i2c_transfer.dataSize       = 1;

  /* Send master non-blocking data to slave */
  reVal = LPI2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &i2c_transfer);
  /*  Reset master completion flag to false. */
  g_MasterCompletionFlag = false;
  if(reVal != kStatus_Success)
  {
    return 1;
  }
  /*  Wait for transfer completed. */
  while(!g_MasterCompletionFlag) {}
  g_MasterCompletionFlag = false;

  return 0;
}

void i2c_read(uint8_t reg_address, uint8_t *buf)
{
  status_t reVal          = kStatus_Fail;
  i2c_transfer.direction  = kLPI2C_Read;
  i2c_transfer.subaddress = reg_address;
  i2c_transfer.data       = buf;
  i2c_transfer.dataSize   = 1U;

  reVal = LPI2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &i2c_transfer);
  if(reVal != kStatus_Success)
  {
    return;
  }
  g_MasterCompletionFlag = false;

  while(!g_MasterCompletionFlag) {}
  g_MasterCompletionFlag = false;

  // PRINTF("Read addr %d: 0x%2x \n", reg_address, buf);
}

int i2c_write_check(uint8_t reg_address, uint8_t reg_value)
{
  uint8_t reg;
  i2c_write(reg_address, reg_value);
  i2c_read(reg_address, &reg);
  PRINTF("Write: 0x%2x, 0x%2x Read: 0x%2x \n", reg_address, reg_value, reg);

  return 0;
}

int codec_init(void)
{
  uint8_t reg;

  i2c_write(AIC3X_PAGE_SELECT, 0);

  if(i2c_write(AIC3X_RESET, 0x80))
    return 0;

  // delay
  int wait_cycles = 100000;
  while(wait_cycles--)
    ;

  // i2c_write(AIC3X_ASD_INTF_CTRLA, 0b00001100);
  i2c_write(AIC3X_ASD_INTF_CTRLA, 0x00); // Slave mode
  i2c_write(AIC3X_ASD_INTF_CTRLB, 0b00110000);
  // i2c_write(AIC3X_ASD_INTF_CTRLC, 0x00); // TODO!!

  /* DAC default volume and mute */
  i2c_write(LDAC_VOL, DEFAULT_VOL | MUTE_ON);
  i2c_write(RDAC_VOL, DEFAULT_VOL | MUTE_ON);

  i2c_write(HPLCOM_CFG, 0xE0);
  i2c_write(HPRCOM_CFG, 0x14);

  // DAC to HP default volume and route to Output mixer */
  //   i2c_write(DACL1_2_HPLOUT_VOL, DEFAULT_VOL | ROUTE_ON);
  //   i2c_write(DACR1_2_HPROUT_VOL, DEFAULT_VOL | ROUTE_ON);
  // Bias set to AVDD
  i2c_write(MICBIAS_CTRL, 0xC0);

  // Line out L sukeista vietom su R todel sujungiam:
  // DAC_R->HP*L*COM ir DAC_L->HP*R*COM
  i2c_write(DACR1_2_HPLCOM_VOL, DEFAULT_VOL | ROUTE_ON);
  i2c_write(DACL1_2_HPRCOM_VOL, DEFAULT_VOL | ROUTE_ON);

  // unmute all outputs
  i2c_read(HPLOUT_CTRL, &reg);
  i2c_write(HPLOUT_CTRL, reg | UNMUTE);
  i2c_read(HPROUT_CTRL, &reg);
  i2c_write(HPROUT_CTRL, reg | UNMUTE);
  //   i2c_read(HPLCOM_CTRL, &reg);
  //   i2c_write(HPLCOM_CTRL, reg | UNMUTE);
  //   i2c_read(HPRCOM_CTRL, &reg);
  //   i2c_write(HPRCOM_CTRL, reg | UNMUTE);

  // ! TEST
  i2c_write(HPRCOM_CTRL, 0b00001001); // Lineout Left (sukeista)
  i2c_write(HPLCOM_CTRL, 0b00001001); // Lineout Right (sukeista)
  //   i2c_write(HPRCOM_CTRL, reg | UNMUTE);

  /* ADC default volume and unmute */
  i2c_write(LADC_VOL, DEFAULT_GAIN);
  i2c_write(RADC_VOL, DEFAULT_GAIN);
  /* By default route Line1 to ADC PGA mixer */
  i2c_write(LINE1L_2_LADC_CTRL, 0x0);
  i2c_write(LINE1R_2_RADC_CTRL, 0x0);
  //

  // PGA to HP Bypass default volume, disconnect from Output Mixer
  i2c_write(PGAL_2_HPLOUT_VOL, 0x00);
  i2c_write(PGAR_2_HPROUT_VOL, 0x00);
  i2c_write(PGAL_2_HPLCOM_VOL, 0x00);
  i2c_write(PGAR_2_HPRCOM_VOL, 0x00);

  i2c_write(AIC3X_CLOCK_REG, 0x01);
  i2c_write(AIC3X_CLKGEN_CTRL_REG, 0x02);
  i2c_write(AIC3X_HEADSET_DETECT_CTRL_B, 0x80);

  i2c_write(AIC3X_CODEC_DFILT_CTRL, 0b01010000);

  // Line in volume up
  i2c_write(MIC3LR_2_LADC_CTRL, 0b00001111);
  i2c_write(MIC3LR_2_RADC_CTRL, 0b11110000);

  // Mic volume down
  //                               |__|
  i2c_write(LINE1R_2_LADC_CTRL, 0b11111000); // LINE1R -> Left ADC
  i2c_write(LINE1R_2_RADC_CTRL, 0b11111100); // LINE1R -> Right ADC and Right ADC ON
  i2c_write(LINE1L_2_LADC_CTRL, 0b01111100); // LINE1L *NOT CONNECTED* and Left ADC ON

  i2c_write(CODEC_HP_OUT_STAGE_CTL, 0x41); //common-mode voltage=1.5V, sof-stepping 2 samples         //register 40
  i2c_write(HPOUT_POP_REDUCTION, 0x80);    //power-on 10ms, ramp up 1ms, resistor divider                     //register 42

  // Power on ADC
  i2c_write(DAC_LINE_MUX, 0b10100000); // L2 path
  //   i2c_write(DAC_LINE_MUX, 0b00000000); // L1 path
  i2c_write(AIC3X_CODEC_DATAPATH_REG, 0b00001010);

  // Unmute DACS
  // Pritildyti DACus
  i2c_write(LDAC_VOL, 0x18); // 0x18 -12 dB
  i2c_write(RDAC_VOL, 0x18); // 0x18 -12 dB

  // HPLOUT/HPROUT:
  // D7-D4 0dB gain
  // D3 unmuted
  // D2 high impedamce when powerd down
  // D1 READ ONLY
  // D0 Fully powered up
  i2c_write(HPLOUT_CTRL, 0x0F);
  i2c_write(HPROUT_CTRL, 0x0F);

  return 1;
}

void codec_enable_line_input(void)
{
  // Mute line in due to weird noises
  i2c_write(MIC3LR_2_LADC_CTRL, 0b00001111);
  i2c_write(MIC3LR_2_RADC_CTRL, 0b11110000);

  i2c_write(LINE1R_2_LADC_CTRL, 0b11111000); // LINE1R -> Left ADC
  i2c_write(LINE1R_2_RADC_CTRL, 0b11111100); // LINE1R -> Right ADC and Right ADC ON
}

void codec_enable_mic_input(void)
{
  i2c_write(MIC3LR_2_LADC_CTRL, 0xFF);
  i2c_write(MIC3LR_2_RADC_CTRL, 0xFF);

  i2c_write(LINE1R_2_LADC_CTRL, 0b10000000); // LINE1R -> Left ADC
  i2c_write(LINE1R_2_RADC_CTRL, 0b10000100); // LINE1R -> Right ADC and Right ADC ON
}

#endif
