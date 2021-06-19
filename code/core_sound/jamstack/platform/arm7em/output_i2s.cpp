#if defined(__ARM_ARCH_7EM__)

#include "output_i2s.h"
#include "memspace.h"
#include "memcpy_audio.h"

sound_block_t *SoundOutputI2S::block_left_1st        = NULL;
sound_block_t *SoundOutputI2S::block_right_1st       = NULL;
sound_block_t *SoundOutputI2S::block_left_2nd        = NULL;
sound_block_t *SoundOutputI2S::block_right_2nd       = NULL;
uint16_t       SoundOutputI2S::block_left_offset     = 0;
uint16_t       SoundOutputI2S::block_right_offset    = 0;
bool           SoundOutputI2S::update_responsibility = false;
DMAChannel     SoundOutputI2S::dma(false);

DMAMEM __attribute__((aligned(32))) static uint32_t i2s_tx_buffer[SOUND_BLOCK_SAMPLES];

#include "audio_helper.h"

void SoundOutputI2S::begin(void)
{
  dma.begin(true); // Allocate the DMA channel first

  block_left_1st  = NULL;
  block_right_1st = NULL;

  config_i2s();

  // CORE_PIN7_CONFIG = 3; //1:TX_DATA0 // todo
  dma.TCD->SADDR         = i2s_tx_buffer;
  dma.TCD->SOFF          = 2;
  dma.TCD->ATTR          = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
  dma.TCD->NBYTES_MLNO   = 2;
  dma.TCD->SLAST         = -sizeof(i2s_tx_buffer);
  dma.TCD->DOFF          = 0;
  dma.TCD->CITER_ELINKNO = sizeof(i2s_tx_buffer) / 2;
  dma.TCD->DLASTSGA      = 0;
  dma.TCD->BITER_ELINKNO = sizeof(i2s_tx_buffer) / 2;
  dma.TCD->CSR           = DMA_TCD_CSR_INTHALF | DMA_TCD_CSR_INTMAJOR;
  dma.TCD->DADDR         = (void *)((uint32_t)&I2S1_TDR0 + 2);
  dma.triggerAtHardwareEvent(DMAMUX_SOURCE_SAI1_TX);
  dma.enable();

  //          Receiver Enable   Bit Clock Enable
  I2S1_RCSR |= I2S_RCSR_RE_ZM | I2S_RCSR_BCE_ZM;
  //          Receiver Enable  Bit Clock Enable  FIFO Request DMA Enable
  I2S1_TCSR = I2S_TCSR_TE_ZM | I2S_TCSR_BCE_ZM | I2S_TCSR_FRDE_ZM;

  update_responsibility = update_setup();
  dma.attachInterrupt(isr, 1U);
}

void SoundOutputI2S::isr(void)
{
  int16_t *      dest;
  sound_block_t *blockL, *blockR;
  uint32_t       saddr, offsetL, offsetR;

  saddr = (uint32_t)(dma.TCD->SADDR);
  dma.clearInterrupt();
  if(saddr < (uint32_t)i2s_tx_buffer + sizeof(i2s_tx_buffer) / 2)
  {
    // DMA is transmitting the first half of the buffer
    // so we must fill the second half
    dest = (int16_t *)&i2s_tx_buffer[SOUND_BLOCK_SAMPLES / 2];
    if(SoundOutputI2S::update_responsibility)
      SoundStream::update_all();
  }
  else
  {
    // DMA is transmitting the second half of the buffer
    // so we must fill the first half
    dest = (int16_t *)i2s_tx_buffer;
  }

  blockL  = SoundOutputI2S::block_left_1st;
  blockR  = SoundOutputI2S::block_right_1st;
  offsetL = SoundOutputI2S::block_left_offset;
  offsetR = SoundOutputI2S::block_right_offset;

  if(blockL && blockR)
  {
    memcpy_tointerleaveLR(dest, blockL->data + offsetL, blockR->data + offsetR);
    offsetL += SOUND_BLOCK_SAMPLES / 2;
    offsetR += SOUND_BLOCK_SAMPLES / 2;
  }
  else if(blockL)
  {
    memcpy_tointerleaveL(dest, blockL->data + offsetL);
    offsetL += SOUND_BLOCK_SAMPLES / 2;
  }
  else if(blockR)
  {
    memcpy_tointerleaveR(dest, blockR->data + offsetR);
    offsetR += SOUND_BLOCK_SAMPLES / 2;
  }
  else
  {
    memset(dest, 0, SOUND_BLOCK_SAMPLES * 2);
  }

  arm_dcache_flush_delete(dest, sizeof(i2s_tx_buffer) / 2);

  if(offsetL < SOUND_BLOCK_SAMPLES)
  {
    SoundOutputI2S::block_left_offset = offsetL;
  }
  else
  {
    SoundOutputI2S::block_left_offset = 0;
    SoundStream::release(blockL);
    SoundOutputI2S::block_left_1st = SoundOutputI2S::block_left_2nd;
    SoundOutputI2S::block_left_2nd = NULL;
  }
  if(offsetR < SOUND_BLOCK_SAMPLES)
  {
    SoundOutputI2S::block_right_offset = offsetR;
  }
  else
  {
    SoundOutputI2S::block_right_offset = 0;
    SoundStream::release(blockR);
    SoundOutputI2S::block_right_1st = SoundOutputI2S::block_right_2nd;
    SoundOutputI2S::block_right_2nd = NULL;
  }
}

void SoundOutputI2S::update(void)
{
  // null audio device: discard all incoming data
  //if (!active) return;
  //sound_block_t *block = receiveReadOnly();
  //if (block) release(block);

  sound_block_t *block;
  block = receiveReadOnly(0); // input 0 = left channel
  if(block)
  {
    __zm_disable_irq();
    if(block_left_1st == NULL)
    {
      block_left_1st    = block;
      block_left_offset = 0;
      __zm_enable_irq();
    }
    else if(block_left_2nd == NULL)
    {
      block_left_2nd = block;
      __zm_enable_irq();
    }
    else
    {
      sound_block_t *tmp = block_left_1st;
      block_left_1st     = block_left_2nd;
      block_left_2nd     = block;
      block_left_offset  = 0;
      __zm_enable_irq();
      release(tmp);
    }
  }
  block = receiveReadOnly(1); // input 1 = right channel
  if(block)
  {
    __zm_disable_irq();
    if(block_right_1st == NULL)
    {
      block_right_1st    = block;
      block_right_offset = 0;
      __zm_enable_irq();
    }
    else if(block_right_2nd == NULL)
    {
      block_right_2nd = block;
      __zm_enable_irq();
    }
    else
    {
      sound_block_t *tmp = block_right_1st;
      block_right_1st    = block_right_2nd;
      block_right_2nd    = block;
      block_right_offset = 0;
      __zm_enable_irq();
      release(tmp);
    }
  }
}

#if 0

void SoundOutputI2S::config_i2s_alt_1176(void)
{
  //* Enable SAI MCLK Output
  IOMUXC_GPR->GPR0 |= IOMUXC_GPR_GPR0_SAI1_MCLK_DIR_MASK;

  //* The following should be done by "CLOCK_InitAudioPll"
  //
  /*     // if either transmitter or receiver is enabled, do nothing
  if(I2S1_TCSR & I2S_TCSR_TE_ZM)
    return;
  if(I2S1_RCSR & I2S_RCSR_RE_ZM)
    return;

  //PLL:
  int fs = SOUND_SAMPLING_RATE_EXACT;
  // PLL between 27*24 = 648MHz and 54*24=1296MHz
  int n1 = 4; // SAI prescaler 4 => (n1*n2) = multiple of 4
  int n2 = 1 + (24000000 * 27) / (fs * 256 * n1);

  double C  = ((double)fs * 256 * n1 * n2) / 24000000;
  int    c0 = C;
  int    c2 = 10000;
  int    c1 = C * c2 - (c0 * c2);
  set_audioClock(c0, c1, c2);

  // clear SAI1_CLK register locations
  CCM_CSCMR1 = (CCM_CSCMR1 & ~(CCM_CSCMR1_SAI1_CLK_SEL_MASK))
               | CCM_CSCMR1_SAI1_CLK_SEL(2); // &0x03 // (0,1,2): PLL3PFD0, PLL5, PLL4
  CCM_CS1CDR = (CCM_CS1CDR & ~(CCM_CS1CDR_SAI1_CLK_PRED_MASK | CCM_CS1CDR_SAI1_CLK_PODF_MASK))
               | CCM_CS1CDR_SAI1_CLK_PRED(n1 - 1)  // &0x07
               | CCM_CS1CDR_SAI1_CLK_PODF(n2 - 1); // &0x3f
  // Select MCLK
  IOMUXC_GPR_GPR1 = (IOMUXC_GPR_GPR1 & ~(IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_MASK_ZM))
                    | (IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_ZM | IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_ZM(0));

  // TODO: Should we configure MCLK, RX_BCLK, RX_SYNC pins here instead BOARD_pinMux? */

  int sai_rx_sync = 0;
  int sai_tx_sync  = 1;

  /*
  ████████╗██╗  ██╗
  ╚══██╔══╝╚██╗██╔╝
      ██║    ╚███╔╝
      ██║    ██╔██╗
      ██║   ██╔╝ ██╗
      ╚═╝   ╚═╝  ╚═╝
  */
  // SAI Transmit Mask Register, offset: 0x60
  I2S1_TMR = 0;

  // TCR1-5 Configuration Registers
  //

  // "SAI Transmit Configuration 1" = "Receive FIFO watermark"(1)
  I2S1_TCR1 = I2S_TCR1_RFW_ZM(1);

  //          0=async 1=sync with receiver
  I2S1_TCR2 = I2S_TCR2_SYNC_ZM(sai_tx_sync) |
              // Bit Clock Polarity
              I2S_TCR2_BCP_ZM |
              // Bit Clock Direction
              //     Bit clock divide by (DIV+1)*2    MCLK select, 0=bus clock, 1=I2S0_MCLK
              //    ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾V‾‾‾‾‾‾    ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              (I2S_TCR2_BCD_ZM | I2S_TCR2_DIV_ZM((1)) | I2S_TCR2_MSEL_ZM(1));

  // receive channel enable
  I2S1_TCR3 = I2S_TCR3_TCE_ZM;

  //          Frame Size                  Sync Width
  //          ‾‾‾‾V‾‾‾‾                  ‾‾‾V‾‾‾‾‾
  I2S1_TCR4 = I2S_TCR4_FRSZ_ZM((2 - 1)) | I2S_TCR4_SYWD_ZM((32 - 1)) |
              // MSB First     Frame Sync Direction  Frame Sync Early   Frame Sync Polarity
              // ‾‾V‾‾‾‾‾     ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾  ‾‾‾‾‾‾V‾‾‾‾‾‾‾‾   ‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              I2S_TCR4_MF_ZM | I2S_TCR4_FSD_ZM | I2S_TCR4_FSE_ZM | I2S_TCR4_FSP_ZM;

  //          Word N Width                Word 0 Width                First Bit Shifted
  I2S1_TCR5 = I2S_TCR5_WNW_ZM((32 - 1)) | I2S_TCR5_W0W_ZM((32 - 1)) | I2S_TCR5_FBT_ZM((32 - 1));

  /*
  ██████╗ ██╗  ██╗
  ██╔══██╗╚██╗██╔╝
  ██████╔╝ ╚███╔╝
  ██╔══██╗ ██╔██╗
  ██║  ██║██╔╝ ██╗
  ╚═╝  ╚═╝╚═╝  ╚═╝
  */
  // SAI Receive Mask Register, offset: 0xE0
  I2S1_RMR = 0;

  // RCR1-5 Configuration Register
  // (this is very similar to TCR)

  // "SAI Receive Configuration 1" = "Receive FIFO watermark"(1)
  I2S1_RCR1 = I2S_RCR1_RFW_ZM(1);
  //          0=async 1=sync with receiver
  I2S1_RCR2 = I2S_RCR2_SYNC_ZM(sai_rx_sync) |
              // Bit Clock Polarity
              I2S_RCR2_BCP_ZM |
              // Bit Clock Direction
              //     Bit clock divide by (DIV+1)*2    MCLK select, 0=bus clock, 1=I2S0_MCLK
              //    ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾V‾‾‾‾‾‾    ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              (I2S_RCR2_BCD_ZM | I2S_RCR2_DIV_ZM((1)) | I2S_RCR2_MSEL_ZM(1));

  // receive channel enable
  I2S1_RCR3 = I2S_RCR3_RCE_ZM;

  //          Frame Size                  Sync Width
  //          ‾‾‾‾V‾‾‾‾                  ‾‾‾V‾‾‾‾‾
  I2S1_RCR4 = I2S_RCR4_FRSZ_ZM((2 - 1)) | I2S_RCR4_SYWD_ZM((32 - 1)) |
              // MSB First     Frame Sync Direction  Frame Sync Early   Frame Sync Polarity
              // ‾‾V‾‾‾‾‾     ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾  ‾‾‾‾‾‾V‾‾‾‾‾‾‾‾   ‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              I2S_RCR4_MF_ZM | I2S_RCR4_FSE_ZM | I2S_RCR4_FSP_ZM | I2S_RCR4_FSD_ZM;

  //          Word N Width                Word 0 Width                First Bit Shifted
  I2S1_RCR5 = I2S_RCR5_WNW_ZM((32 - 1)) | I2S_RCR5_W0W_ZM((32 - 1)) | I2S_RCR5_FBT_ZM((32 - 1));
}

#endif

void SoundOutputI2S::config_i2s(void)
{
  CCM_CCGR5 |= CCM_CCGR5_SAI1(CCM_CCGR_ON);

  // if either transmitter or receiver is enabled, do nothing
  if(I2S1_TCSR & I2S_TCSR_TE_ZM)
    return;
  if(I2S1_RCSR & I2S_RCSR_RE_ZM)
    return;

  //PLL:
  int fs = SOUND_SAMPLING_RATE_EXACT;
  // PLL between 27*24 = 648MHz and 54*24=1296MHz
  int n1 = 4; // SAI prescaler 4 => (n1*n2) = multiple of 4
  int n2 = 1 + (24000000 * 27) / (fs * 256 * n1);

  double C  = ((double)fs * 256 * n1 * n2) / 24000000;
  int    c0 = C;
  int    c2 = 10000;
  int    c1 = C * c2 - (c0 * c2);
  set_audioClock(c0, c1, c2);

  // clear SAI1_CLK register locations
  CCM_CSCMR1 = (CCM_CSCMR1 & ~(CCM_CSCMR1_SAI1_CLK_SEL_MASK))
               | CCM_CSCMR1_SAI1_CLK_SEL(2); // &0x03 // (0,1,2): PLL3PFD0, PLL5, PLL4
  CCM_CS1CDR = (CCM_CS1CDR & ~(CCM_CS1CDR_SAI1_CLK_PRED_MASK | CCM_CS1CDR_SAI1_CLK_PODF_MASK))
               | CCM_CS1CDR_SAI1_CLK_PRED(n1 - 1)  // &0x07
               | CCM_CS1CDR_SAI1_CLK_PODF(n2 - 1); // &0x3f
  // Select MCLK
  IOMUXC_GPR_GPR1 = (IOMUXC_GPR_GPR1 & ~(IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_MASK_ZM))
                    | (IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_ZM | IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_ZM(0));

  // TODO: Should we configure MCLK, RX_BCLK, RX_SYNC pins here instead BOARD_pinMux?

  int sai_rx_sync = 0;
  int sai_tx_sync = 1;

  /*
  ████████╗██╗  ██╗
  ╚══██╔══╝╚██╗██╔╝
      ██║    ╚███╔╝
      ██║    ██╔██╗
      ██║   ██╔╝ ██╗
      ╚═╝   ╚═╝  ╚═╝
  */
  // SAI Transmit Mask Register, offset: 0x60
  I2S1_TMR = 0;

  // TCR1-5 Configuration Registers
  //

  // "SAI Transmit Configuration 1" = "Receive FIFO watermark"(1)
  I2S1_TCR1 = I2S_TCR1_RFW_ZM(1);

  //          0=async 1=sync with receiver
  I2S1_TCR2 = I2S_TCR2_SYNC_ZM(sai_tx_sync) |
              // Bit Clock Polarity
              I2S_TCR2_BCP_ZM |
              // Bit Clock Direction
              //     Bit clock divide by (DIV+1)*2    MCLK select, 0=bus clock, 1=I2S0_MCLK
              //    ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾V‾‾‾‾‾‾    ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              (I2S_TCR2_BCD_ZM | I2S_TCR2_DIV_ZM((1)) | I2S_TCR2_MSEL_ZM(1));

  // receive channel enable
  I2S1_TCR3 = I2S_TCR3_TCE_ZM;

  //          Frame Size                  Sync Width
  //          ‾‾‾‾V‾‾‾‾                  ‾‾‾V‾‾‾‾‾
  I2S1_TCR4 = I2S_TCR4_FRSZ_ZM((2 - 1)) | I2S_TCR4_SYWD_ZM((32 - 1)) |
              // MSB First     Frame Sync Direction  Frame Sync Early   Frame Sync Polarity
              // ‾‾V‾‾‾‾‾     ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾  ‾‾‾‾‾‾V‾‾‾‾‾‾‾‾   ‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              I2S_TCR4_MF_ZM | I2S_TCR4_FSD_ZM | I2S_TCR4_FSE_ZM | I2S_TCR4_FSP_ZM;

  //          Word N Width                Word 0 Width                First Bit Shifted
  I2S1_TCR5 = I2S_TCR5_WNW_ZM((32 - 1)) | I2S_TCR5_W0W_ZM((32 - 1)) | I2S_TCR5_FBT_ZM((32 - 1));

  /*
  ██████╗ ██╗  ██╗
  ██╔══██╗╚██╗██╔╝
  ██████╔╝ ╚███╔╝
  ██╔══██╗ ██╔██╗
  ██║  ██║██╔╝ ██╗
  ╚═╝  ╚═╝╚═╝  ╚═╝
  */
  // SAI Receive Mask Register, offset: 0xE0
  I2S1_RMR = 0;

  // RCR1-5 Configuration Register
  // (this is very similar to TCR)

  // "SAI Receive Configuration 1" = "Receive FIFO watermark"(1)
  I2S1_RCR1 = I2S_RCR1_RFW_ZM(1);
  //          0=async 1=sync with receiver
  I2S1_RCR2 = I2S_RCR2_SYNC_ZM(sai_rx_sync) |
              // Bit Clock Polarity
              I2S_RCR2_BCP_ZM |
              // Bit Clock Direction
              //     Bit clock divide by (DIV+1)*2    MCLK select, 0=bus clock, 1=I2S0_MCLK
              //    ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾V‾‾‾‾‾‾    ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              (I2S_RCR2_BCD_ZM | I2S_RCR2_DIV_ZM((1)) | I2S_RCR2_MSEL_ZM(1));

  // receive channel enable
  I2S1_RCR3 = I2S_RCR3_RCE_ZM;

  //          Frame Size                  Sync Width
  //          ‾‾‾‾V‾‾‾‾                  ‾‾‾V‾‾‾‾‾
  I2S1_RCR4 = I2S_RCR4_FRSZ_ZM((2 - 1)) | I2S_RCR4_SYWD_ZM((32 - 1)) |
              // MSB First     Frame Sync Direction  Frame Sync Early   Frame Sync Polarity
              // ‾‾V‾‾‾‾‾     ‾‾‾‾‾‾‾‾V‾‾‾‾‾‾‾‾‾  ‾‾‾‾‾‾V‾‾‾‾‾‾‾‾   ‾‾V‾‾‾‾‾‾‾‾‾‾‾‾‾‾
              I2S_RCR4_MF_ZM | I2S_RCR4_FSE_ZM | I2S_RCR4_FSP_ZM | I2S_RCR4_FSD_ZM;

  //          Word N Width                Word 0 Width                First Bit Shifted
  I2S1_RCR5 = I2S_RCR5_WNW_ZM((32 - 1)) | I2S_RCR5_W0W_ZM((32 - 1)) | I2S_RCR5_FBT_ZM((32 - 1));
}

#endif
