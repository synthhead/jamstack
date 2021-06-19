#include "input_i2s.h"
#include "output_i2s.h"
#include "memspace.h"

DMAMEM __attribute__((aligned(32))) static uint32_t i2s_rx_buffer[SOUND_BLOCK_SAMPLES];

sound_block_t* SoundInputI2S::block_left            = NULL;
sound_block_t* SoundInputI2S::block_right           = NULL;
uint16_t       SoundInputI2S::block_offset          = 0;
bool           SoundInputI2S::update_responsibility = false;
DMAChannel     SoundInputI2S::dma(false);

void SoundInputI2S::begin(void)
{
  dma.begin(true); // Allocate the DMA channel first

  //block_left_1st = NULL;
  //block_right_1st = NULL;

  // TODO: should we set & clear the I2S_RCSR_SR bit here?
  SoundOutputI2S::config_i2s();

  // CORE_PIN8_CONFIG = 3; //1:RX_DATA0
  IOMUXC_SAI1_RX_DATA0_SELECT_INPUT = 2;

  dma.TCD->SADDR         = (void*)((uint32_t)&I2S1_RDR0 + 2);
  dma.TCD->SOFF          = 0;
  dma.TCD->ATTR          = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
  dma.TCD->NBYTES_MLNO   = 2;
  dma.TCD->SLAST         = 0;
  dma.TCD->DADDR         = i2s_rx_buffer;
  dma.TCD->DOFF          = 2;
  dma.TCD->CITER_ELINKNO = sizeof(i2s_rx_buffer) / 2;
  dma.TCD->DLASTSGA      = -sizeof(i2s_rx_buffer);
  dma.TCD->BITER_ELINKNO = sizeof(i2s_rx_buffer) / 2;
  dma.TCD->CSR           = DMA_TCD_CSR_INTHALF | DMA_TCD_CSR_INTMAJOR;
  dma.triggerAtHardwareEvent(DMAMUX_SOURCE_SAI1_RX);

  I2S1_RCSR = I2S_RCSR_RE_ZM | I2S_RCSR_BCE_ZM | I2S_RCSR_FRDE_ZM | I2S_RCSR_FR_ZM;

  update_responsibility = update_setup();
  dma.enable();
  dma.attachInterrupt(isr, 2U);
}

void SoundInputI2S::isr(void)
{
  uint32_t       daddr, offset;
  const int16_t *src, *end;
  int16_t *      dest_left, *dest_right;
  sound_block_t *left, *right;

  daddr = (uint32_t)(dma.TCD->DADDR);
  dma.clearInterrupt();
  //Serial.println("isr");

  if(daddr < (uint32_t)i2s_rx_buffer + sizeof(i2s_rx_buffer) / 2)
  {
    // DMA is receiving to the first half of the buffer
    // need to remove data from the second half
    src = (int16_t*)&i2s_rx_buffer[SOUND_BLOCK_SAMPLES / 2];
    end = (int16_t*)&i2s_rx_buffer[SOUND_BLOCK_SAMPLES];
    if(SoundInputI2S::update_responsibility)
      SoundStream::update_all();
  }
  else
  {
    // DMA is receiving to the second half of the buffer
    // need to remove data from the first half
    src = (int16_t*)&i2s_rx_buffer[0];
    end = (int16_t*)&i2s_rx_buffer[SOUND_BLOCK_SAMPLES / 2];
  }
  left  = SoundInputI2S::block_left;
  right = SoundInputI2S::block_right;
  if(left != NULL && right != NULL)
  {
    offset = SoundInputI2S::block_offset;
    if(offset <= SOUND_BLOCK_SAMPLES / 2)
    {
      dest_left                   = &(left->data[offset]);
      dest_right                  = &(right->data[offset]);
      SoundInputI2S::block_offset = offset + SOUND_BLOCK_SAMPLES / 2;
      arm_dcache_delete((void*)src, sizeof(i2s_rx_buffer) / 2);
      do
      {
        *dest_left++  = *src++;
        *dest_right++ = *src++;
      } while(src < end);
    }
  }
}

void SoundInputI2S::update(void)
{
  sound_block_t *new_left = NULL, *new_right = NULL, *out_left = NULL, *out_right = NULL;

  // allocate 2 new blocks, but if one fails, allocate neither
  new_left = allocate();
  if(new_left != NULL)
  {
    new_right = allocate();
    if(new_right == NULL)
    {
      release(new_left);
      new_left = NULL;
    }
  }
  __disable_irq();
  if(block_offset >= SOUND_BLOCK_SAMPLES)
  {
    // the DMA filled 2 blocks, so grab them and get the
    // 2 new blocks to the DMA, as quickly as possible
    out_left     = block_left;
    block_left   = new_left;
    out_right    = block_right;
    block_right  = new_right;
    block_offset = 0;
    __enable_irq();
    // then transmit the DMA's former blocks
    transmit(out_left, 0);
    release(out_left);
    transmit(out_right, 1);
    release(out_right);
    //Serial.print(".");
  }
  else if(new_left != NULL)
  {
    // the DMA didn't fill blocks, but we allocated blocks
    if(block_left == NULL)
    {
      // the DMA doesn't have any blocks to fill, so
      // give it the ones we just allocated
      block_left   = new_left;
      block_right  = new_right;
      block_offset = 0;
      __enable_irq();
    }
    else
    {
      // the DMA already has blocks, doesn't need these
      __enable_irq();
      release(new_left);
      release(new_right);
    }
  }
  else
  {
    // The DMA didn't fill blocks, and we could not allocate
    // memory... the system is likely starving for memory!
    // Sadly, there's nothing we can do.
    __enable_irq();
  }
}
