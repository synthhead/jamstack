#include "DMAChannel.h"

#define DMA_MAX_CHANNELS 16

uint16_t dma_channel_allocated_mask = 0;
void (*isrFuncs[2])(void);

#ifdef CR
#warning "CR is defined as something?"
#endif

void DMAChannel::begin(bool force_initialization)
{
  uint32_t ch = 0;

  __zm_disable_irq();
  if(!force_initialization && TCD && channel < DMA_MAX_CHANNELS && (dma_channel_allocated_mask & (1 << channel))
     && (uint32_t)TCD == (uint32_t)(0x400E9000 + channel * 32))
  {
    __zm_enable_irq();
    return;
  }
  while(1)
  {
    if(!(dma_channel_allocated_mask & (1 << ch)))
    {
      dma_channel_allocated_mask |= (1 << ch);
      __zm_enable_irq();
      break;
    }
    if(++ch >= DMA_MAX_CHANNELS)
    {
      __zm_enable_irq();
      TCD     = (TCD_t *)0;
      channel = DMA_MAX_CHANNELS;
      return;
    }
  }
  channel = ch;

  CCM_CCGR5 |= CCM_CCGR5_DMA(CCM_CCGR_ON);
  DMA_CR      = DMA_CR_GRP1PRI_ZM | DMA_CR_EMLM_ZM | DMA_CR_EDBG_ZM;
  DMA_CERQ    = ch;
  DMA_CERR    = ch;
  DMA_CEEI    = ch;
  DMA_CINT    = ch;
  TCD         = (TCD_t *)(0x400E9000 + ch * 32);
  uint32_t *p = (uint32_t *)TCD;
  *p++        = 0;
  *p++        = 0;
  *p++        = 0;
  *p++        = 0;
  *p++        = 0;
  *p++        = 0;
  *p++        = 0;
  *p++        = 0;
}

void DMAChannel::release(void)
{
  if(channel >= DMA_MAX_CHANNELS)
    return;
  DMA_CERQ = channel;
  __zm_disable_irq();
  dma_channel_allocated_mask &= ~(1 << channel);
  __zm_enable_irq();
  channel = DMA_MAX_CHANNELS;
  TCD     = (TCD_t *)0;
}

void DMAChannel::attachInterrupt(void (*isr)(void), uint8_t noteToMyself)
{
  isrFuncs[channel] = isr;
  NVIC_EnableIRQ((IRQn)(DMA0_DMA16_IRQn + channel));
}

static uint32_t priority(const DMAChannel &c)
{
  uint32_t n;
  n = *(uint32_t *)((uint32_t)&DMA_DCHPRI3 + (c.channel & 0xFC));
  n = __builtin_bswap32(n);
  return (n >> ((c.channel & 0x03) << 3)) & 0x0F;
}

static void swap(DMAChannel &c1, DMAChannel &c2)
{
  uint8_t              c;
  DMABaseClass::TCD_t *t;

  c          = c1.channel;
  c1.channel = c2.channel;
  c2.channel = c;
  t          = c1.TCD;
  c1.TCD     = c2.TCD;
  c2.TCD     = t;
}

void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2)
{
  if(priority(ch1) < priority(ch2))
    swap(ch1, ch2);
}

void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3)
{
  if(priority(ch2) < priority(ch3))
    swap(ch2, ch3);
  if(priority(ch1) < priority(ch2))
    swap(ch1, ch2);
  if(priority(ch2) < priority(ch3))
    swap(ch2, ch3);
}

void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3, DMAChannel &ch4)
{
  if(priority(ch3) < priority(ch4))
    swap(ch3, ch4);
  if(priority(ch2) < priority(ch3))
    swap(ch2, ch3);
  if(priority(ch1) < priority(ch2))
    swap(ch1, ch2);
  if(priority(ch3) < priority(ch4))
    swap(ch2, ch3);
  if(priority(ch2) < priority(ch3))
    swap(ch1, ch2);
  if(priority(ch3) < priority(ch4))
    swap(ch2, ch3);
}

void DMA0_DMA16_DriverIRQHandler(void)
{
  isrFuncs[0]();
  __DSB();
}

void DMA1_DMA17_DriverIRQHandler(void)
{
  isrFuncs[1]();
  __DSB();
}
