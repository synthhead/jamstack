#ifndef sdram_h_
#define sdram_h_

#include "MIMXRT1021.h"
#include "fsl_debug_console.h"
#include "fsl_semc.h"

#define EXAMPLE_SEMC_START_ADDRESS (0x80001000U)
#define EXAMPLE_SEMC_CLK_FREQ CLOCK_GetFreq(kCLOCK_SemcClk)

#define SEMC_EXAMPLE_DATALEN (0x10U)

extern status_t BOARD_InitSEMC(void);
static void     SEMC_SDRAMReadWrite32Bit(void);
static void     SEMC_SDRAMReadWrite16Bit(void);
static void     SEMC_SDRAMReadWrite8Bit(void);

uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

status_t BOARD_InitSEMC(void)
{
  semc_config_t       config;
  semc_sdram_config_t sdramconfig;
  uint32_t            clockFrq = EXAMPLE_SEMC_CLK_FREQ;

  /* Initializes the MAC configure structure to zero. */
  memset(&config, 0, sizeof(semc_config_t));
  memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

  /* Initialize SEMC. */
  SEMC_GetDefaultConfig(&config);
  config.dqsMode = kSEMC_Loopbackdqspad; /* For more accurate timing. */
  SEMC_Init(SEMC, &config);

  /* Configure SDRAM. */
  sdramconfig.csxPinMux              = kSEMC_MUXCSX0;
  sdramconfig.address                = 0x80000000;
  sdramconfig.memsize_kbytes         = 32 * 1024; /* 32MB = 32*1024*1KBytes*/
  sdramconfig.portSize               = kSEMC_PortSize16Bit;
  sdramconfig.burstLen               = kSEMC_Sdram_BurstLen1;
  sdramconfig.columnAddrBitNum       = kSEMC_SdramColunm_9bit;
  sdramconfig.casLatency             = kSEMC_LatencyThree;
  sdramconfig.tPrecharge2Act_Ns      = 18; /* Trp 18ns */
  sdramconfig.tAct2ReadWrite_Ns      = 18; /* Trcd 18ns */
  sdramconfig.tRefreshRecovery_Ns    = (60 + 67);
  sdramconfig.tWriteRecovery_Ns      = 12; /* 12ns */
  sdramconfig.tCkeOff_Ns             = 42; /* The minimum cycle of SDRAM CLK off state. CKE is off in self refresh at a minimum period tRAS.*/
  sdramconfig.tAct2Prechage_Ns       = 42; /* Tras 42ns */
  sdramconfig.tSelfRefRecovery_Ns    = 67;
  sdramconfig.tRefresh2Refresh_Ns    = 60;
  sdramconfig.tAct2Act_Ns            = 60;
  sdramconfig.tPrescalePeriod_Ns     = 160 /* * (1000000000 / (clockFrq=163862064))*/; // was calc 976
  sdramconfig.refreshPeriod_nsPerRow = 64 /* * 1000000 / 8192 */; /* 64ms/8192 */      // was calc 7812
  sdramconfig.refreshUrgThreshold    = sdramconfig.refreshPeriod_nsPerRow;
  sdramconfig.refreshBurstLen        = 1;

  return SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
}

void SEMC_SDRAMReadWrite32Bit(void)
{
  uint32_t  index;
  uint32_t  datalen = SEMC_EXAMPLE_DATALEN;
  uint32_t *sdram   = (uint32_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
  bool      result  = true;

  /* Prepare data and write to SDRAM. */
  for(index = 0; index < datalen; index++)
  {
    sdram_writeBuffer[index] = index;
    sdram[index]             = sdram_writeBuffer[index];
  }

  /* Read data from the SDRAM. */
  for(index = 0; index < datalen; index++)
  {
    sdram_readBuffer[index] = sdram[index];
  }

  PRINTF("Readout:\n");
  for(uint32_t i = 0; i < datalen; i++)
  {
    PRINTF("%08X: %d\t", sdram + i, sdram_readBuffer[i]);
  }
  PRINTF("\n");

  /* Compare the two buffers. */
  while(datalen--)
  {
    if(sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
    {
      result = false;
      break;
    }
  }

  if(!result)
  {
    PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Failed!\r\n");
  }
  else
  {
    PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Succeed!\r\n");
  }
}

static void SEMC_SDRAMReadWrite16Bit(void)
{
  uint32_t  index;
  uint32_t  datalen = SEMC_EXAMPLE_DATALEN;
  uint16_t *sdram   = (uint16_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
  bool      result  = true;

  memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
  memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

  /* Prepare data and write to SDRAM. */
  for(index = 0; index < datalen; index++)
  {
    sdram_writeBuffer[index] = index % 0xFFFF;
    sdram[index]             = sdram_writeBuffer[index];
  }

  /* Read data from the SDRAM. */
  for(index = 0; index < datalen; index++)
  {
    sdram_readBuffer[index] = sdram[index];
  }

  PRINTF("Readout:\n");
  for(uint32_t i = 0; i < datalen; i++)
  {
    PRINTF("%08X: %d\t", sdram + i, sdram_readBuffer[i]);
  }
  PRINTF("\n");

  /* Compare the two buffers. */
  while(datalen--)
  {
    if(sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
    {
      result = false;
      break;
    }
  }

  if(!result)
  {
    PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Failed!\r\n");
  }
  else
  {
    PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Succeed!\r\n");
  }
}

static void SEMC_SDRAMReadWrite8Bit(void)
{
  uint32_t index;
  uint32_t datalen = SEMC_EXAMPLE_DATALEN;
  uint8_t *sdram   = (uint8_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
  bool     result  = true;

  memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
  memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

  /* Prepare data and write to SDRAM. */
  for(index = 0; index < datalen; index++)
  {
    sdram_writeBuffer[index] = index % 0x100;
    sdram[index]             = sdram_writeBuffer[index];
  }

  /* Read data from the SDRAM. */
  for(index = 0; index < datalen; index++)
  {
    sdram_readBuffer[index] = sdram[index];
  }

  PRINTF("Readout:\n");
  for(uint32_t i = 0; i < datalen; i++)
  {
    PRINTF("%08X: %d\t", sdram + i, sdram_readBuffer[i]);
  }
  PRINTF("\n");

  /* Compare the two buffers. */
  while(datalen--)
  {
    if(sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
    {
      result = false;
      break;
    }
  }

  if(!result)
  {
    PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Failed!\r\n");
  }
  else
  {
    PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Succeed!\r\n");
  }
}

#endif
