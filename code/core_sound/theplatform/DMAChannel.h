#pragma once
#ifndef DMAChannel_h_
#define DMAChannel_h_

#include "MIMXRT1021.h"
#include "fsl_debug_console.h"
#include "micro1020.h"

#define DMACHANNEL_HAS_BEGIN
#define DMACHANNEL_HAS_BOOLEAN_CTOR

#ifdef __cplusplus
extern "C"
{
#endif
  extern uint16_t dma_channel_allocated_mask;
  extern void     DMA0_DMA16_DriverIRQHandler(void);
  extern void     DMA1_DMA17_DriverIRQHandler(void);

  extern void (*isrFunc)(void);
  extern void (*isrFuncI2sInput)(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class DMABaseClass
{
public:
  typedef struct __attribute__((packed, aligned(4)))
  {
    volatile const void *volatile SADDR;
    int16_t SOFF;
    union
    {
      uint16_t ATTR;
      struct
      {
        uint8_t ATTR_DST;
        uint8_t ATTR_SRC;
      };
    };
    union
    {
      uint32_t NBYTES;
      uint32_t NBYTES_MLNO;
      uint32_t NBYTES_MLOFFNO;
      uint32_t NBYTES_MLOFFYES;
    };
    int32_t SLAST;
    volatile void *volatile DADDR;
    int16_t DOFF;
    union
    {
      volatile uint16_t CITER;
      volatile uint16_t CITER_ELINKYES;
      volatile uint16_t CITER_ELINKNO;
    };
    int32_t           DLASTSGA;
    volatile uint16_t CSR;
    union
    {
      volatile uint16_t BITER;
      volatile uint16_t BITER_ELINKYES;
      volatile uint16_t BITER_ELINKNO;
    };
  } TCD_t;
  TCD_t *TCD;

  /***************************************/
  /**    Data Transfer                  **/
  /***************************************/

  void source(volatile const signed char &p) { source(*(volatile const uint8_t *)&p); }
  void source(volatile const unsigned char &p)
  {
    TCD->SADDR    = &p;
    TCD->SOFF     = 0;
    TCD->ATTR_SRC = 0;
    if((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0)
      TCD->NBYTES = 1;
    TCD->SLAST = 0;
  }
  void source(volatile const signed short &p) { source(*(volatile const uint16_t *)&p); }
  void source(volatile const unsigned short &p)
  {
    TCD->SADDR    = &p;
    TCD->SOFF     = 0;
    TCD->ATTR_SRC = 1;
    if((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0)
      TCD->NBYTES = 2;
    TCD->SLAST = 0;
  }
  void source(volatile const signed int &p) { source(*(volatile const uint32_t *)&p); }
  void source(volatile const unsigned int &p) { source(*(volatile const uint32_t *)&p); }
  void source(volatile const signed long &p) { source(*(volatile const uint32_t *)&p); }
  void source(volatile const unsigned long &p)
  {
    TCD->SADDR    = &p;
    TCD->SOFF     = 0;
    TCD->ATTR_SRC = 2;
    if((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0)
      TCD->NBYTES = 4;
    TCD->SLAST = 0;
  }

  void sourceBuffer(volatile const signed char p[], unsigned int len)
  {
    sourceBuffer((volatile const uint8_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned char p[], unsigned int len)
  {
    TCD->SADDR    = p;
    TCD->SOFF     = 1;
    TCD->ATTR_SRC = 0;
    TCD->NBYTES   = 1;
    TCD->SLAST    = -len;
    TCD->BITER    = len;
    TCD->CITER    = len;
  }
  void sourceBuffer(volatile const signed short p[], unsigned int len)
  {
    sourceBuffer((volatile const uint16_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned short p[], unsigned int len)
  {
    TCD->SADDR    = p;
    TCD->SOFF     = 2;
    TCD->ATTR_SRC = 1;
    TCD->NBYTES   = 2;
    TCD->SLAST    = -len;
    TCD->BITER    = len / 2;
    TCD->CITER    = len / 2;
  }
  void sourceBuffer(volatile const signed int p[], unsigned int len)
  {
    sourceBuffer((volatile const uint32_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned int p[], unsigned int len)
  {
    sourceBuffer((volatile const uint32_t *)p, len);
  }
  void sourceBuffer(volatile const signed long p[], unsigned int len)
  {
    sourceBuffer((volatile const uint32_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned long p[], unsigned int len)
  {
    TCD->SADDR    = p;
    TCD->SOFF     = 4;
    TCD->ATTR_SRC = 2;
    TCD->NBYTES   = 4;
    TCD->SLAST    = -len;
    TCD->BITER    = len / 4;
    TCD->CITER    = len / 4;
  }

  void sourceCircular(volatile const signed char p[], unsigned int len)
  {
    sourceCircular((volatile const uint8_t *)p, len);
  }
  void sourceCircular(volatile const unsigned char p[], unsigned int len)
  {
    TCD->SADDR    = p;
    TCD->SOFF     = 1;
    TCD->ATTR_SRC = ((31 - __builtin_clz(len)) << 3);
    TCD->NBYTES   = 1;
    TCD->SLAST    = 0;
    TCD->BITER    = len;
    TCD->CITER    = len;
  }
  void sourceCircular(volatile const signed short p[], unsigned int len)
  {
    sourceCircular((volatile const uint16_t *)p, len);
  }
  void sourceCircular(volatile const unsigned short p[], unsigned int len)
  {
    TCD->SADDR    = p;
    TCD->SOFF     = 2;
    TCD->ATTR_SRC = ((31 - __builtin_clz(len)) << 3) | 1;
    TCD->NBYTES   = 2;
    TCD->SLAST    = 0;
    TCD->BITER    = len / 2;
    TCD->CITER    = len / 2;
  }
  void sourceCircular(volatile const signed int p[], unsigned int len)
  {
    sourceCircular((volatile const uint32_t *)p, len);
  }
  void sourceCircular(volatile const unsigned int p[], unsigned int len)
  {
    sourceCircular((volatile const uint32_t *)p, len);
  }
  void sourceCircular(volatile const signed long p[], unsigned int len)
  {
    sourceCircular((volatile const uint32_t *)p, len);
  }
  void sourceCircular(volatile const unsigned long p[], unsigned int len)
  {
    TCD->SADDR    = p;
    TCD->SOFF     = 4;
    TCD->ATTR_SRC = ((31 - __builtin_clz(len)) << 3) | 2;
    TCD->NBYTES   = 4;
    TCD->SLAST    = 0;
    TCD->BITER    = len / 4;
    TCD->CITER    = len / 4;
  }

  void destination(volatile signed char &p) { destination(*(volatile uint8_t *)&p); }
  void destination(volatile unsigned char &p)
  {
    TCD->DADDR    = &p;
    TCD->DOFF     = 0;
    TCD->ATTR_DST = 0;
    if((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0)
      TCD->NBYTES = 1;
    TCD->DLASTSGA = 0;
  }
  void destination(volatile signed short &p) { destination(*(volatile uint16_t *)&p); }
  void destination(volatile unsigned short &p)
  {
    TCD->DADDR    = &p;
    TCD->DOFF     = 0;
    TCD->ATTR_DST = 1;
    if((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0)
      TCD->NBYTES = 2;
    TCD->DLASTSGA = 0;
  }
  void destination(volatile signed int &p) { destination(*(volatile uint32_t *)&p); }
  void destination(volatile unsigned int &p) { destination(*(volatile uint32_t *)&p); }
  void destination(volatile signed long &p) { destination(*(volatile uint32_t *)&p); }
  void destination(volatile unsigned long &p)
  {
    TCD->DADDR    = &p;
    TCD->DOFF     = 0;
    TCD->ATTR_DST = 2;
    if((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0)
      TCD->NBYTES = 4;
    TCD->DLASTSGA = 0;
  }

  void destinationBuffer(volatile signed char p[], unsigned int len) { destinationBuffer((volatile uint8_t *)p, len); }
  void destinationBuffer(volatile unsigned char p[], unsigned int len)
  {
    TCD->DADDR    = p;
    TCD->DOFF     = 1;
    TCD->ATTR_DST = 0;
    TCD->NBYTES   = 1;
    TCD->DLASTSGA = -len;
    TCD->BITER    = len;
    TCD->CITER    = len;
  }
  void destinationBuffer(volatile signed short p[], unsigned int len)
  {
    destinationBuffer((volatile uint16_t *)p, len);
  }
  void destinationBuffer(volatile unsigned short p[], unsigned int len)
  {
    TCD->DADDR    = p;
    TCD->DOFF     = 2;
    TCD->ATTR_DST = 1;
    TCD->NBYTES   = 2;
    TCD->DLASTSGA = -len;
    TCD->BITER    = len / 2;
    TCD->CITER    = len / 2;
  }
  void destinationBuffer(volatile signed int p[], unsigned int len) { destinationBuffer((volatile uint32_t *)p, len); }
  void destinationBuffer(volatile unsigned int p[], unsigned int len)
  {
    destinationBuffer((volatile uint32_t *)p, len);
  }
  void destinationBuffer(volatile signed long p[], unsigned int len) { destinationBuffer((volatile uint32_t *)p, len); }
  void destinationBuffer(volatile unsigned long p[], unsigned int len)
  {
    TCD->DADDR    = p;
    TCD->DOFF     = 4;
    TCD->ATTR_DST = 2;
    TCD->NBYTES   = 4;
    TCD->DLASTSGA = -len;
    TCD->BITER    = len / 4;
    TCD->CITER    = len / 4;
  }

  void destinationCircular(volatile signed char p[], unsigned int len)
  {
    destinationCircular((volatile uint8_t *)p, len);
  }
  void destinationCircular(volatile unsigned char p[], unsigned int len)
  {
    TCD->DADDR    = p;
    TCD->DOFF     = 1;
    TCD->ATTR_DST = ((31 - __builtin_clz(len)) << 3);
    TCD->NBYTES   = 1;
    TCD->DLASTSGA = 0;
    TCD->BITER    = len;
    TCD->CITER    = len;
  }
  void destinationCircular(volatile signed short p[], unsigned int len)
  {
    destinationCircular((volatile uint16_t *)p, len);
  }
  void destinationCircular(volatile unsigned short p[], unsigned int len)
  {
    TCD->DADDR    = p;
    TCD->DOFF     = 2;
    TCD->ATTR_DST = ((31 - __builtin_clz(len)) << 3) | 1;
    TCD->NBYTES   = 2;
    TCD->DLASTSGA = 0;
    TCD->BITER    = len / 2;
    TCD->CITER    = len / 2;
  }
  void destinationCircular(volatile signed int p[], unsigned int len)
  {
    destinationCircular((volatile uint32_t *)p, len);
  }
  void destinationCircular(volatile unsigned int p[], unsigned int len)
  {
    destinationCircular((volatile uint32_t *)p, len);
  }
  void destinationCircular(volatile signed long p[], unsigned int len)
  {
    destinationCircular((volatile uint32_t *)p, len);
  }
  void destinationCircular(volatile unsigned long p[], unsigned int len)
  {
    TCD->DADDR    = p;
    TCD->DOFF     = 4;
    TCD->ATTR_DST = ((31 - __builtin_clz(len)) << 3) | 2;
    TCD->NBYTES   = 4;
    TCD->DLASTSGA = 0;
    TCD->BITER    = len / 4;
    TCD->CITER    = len / 4;
  }

  /*************************************************/
  /**    Quantity of Data to Transfer             **/
  /*************************************************/

  void transferSize(unsigned int len)
  {
    if(len == 16)
    {
      TCD->NBYTES = 16;
      if(TCD->SOFF != 0)
        TCD->SOFF = 16;
      if(TCD->DOFF != 0)
        TCD->DOFF = 16;
      TCD->ATTR = (TCD->ATTR & 0xF8F8) | 0x0404;
    }
    else if(len == 4)
    {
      TCD->NBYTES = 4;
      if(TCD->SOFF != 0)
        TCD->SOFF = 4;
      if(TCD->DOFF != 0)
        TCD->DOFF = 4;
      TCD->ATTR = (TCD->ATTR & 0xF8F8) | 0x0202;
    }
    else if(len == 2)
    {
      TCD->NBYTES = 2;
      if(TCD->SOFF != 0)
        TCD->SOFF = 2;
      if(TCD->DOFF != 0)
        TCD->DOFF = 2;
      TCD->ATTR = (TCD->ATTR & 0xF8F8) | 0x0101;
    }
    else
    {
      TCD->NBYTES = 1;
      if(TCD->SOFF != 0)
        TCD->SOFF = 1;
      if(TCD->DOFF != 0)
        TCD->DOFF = 1;
      TCD->ATTR = TCD->ATTR & 0xF8F8;
    }
  }

  void transferCount(unsigned int len)
  {
    if(!(TCD->BITER & DMA_TCD_BITER_ELINK))
    {
      if(len > 32767)
        return;
      TCD->BITER = len;
      TCD->CITER = len;
    }
    else
    {
      if(len > 511)
        return;
      TCD->BITER = (TCD->BITER & 0xFE00) | len;
      TCD->CITER = (TCD->CITER & 0xFE00) | len;
    }
  }

  /*************************************************/
  /**    Special Options / Features               **/
  /*************************************************/

  void interruptAtCompletion(void) { TCD->CSR |= DMA_TCD_CSR_INTMAJOR; }

  void interruptAtHalf(void) { TCD->CSR |= DMA_TCD_CSR_INTHALF; }

  void disableOnCompletion(void) { TCD->CSR |= DMA_TCD_CSR_DREQ; }

  void replaceSettingsOnCompletion(const DMABaseClass &settings)
  {
    TCD->DLASTSGA = (int32_t)(settings.TCD);
    TCD->CSR &= ~DMA_TCD_CSR_DONE;
    TCD->CSR |= DMA_TCD_CSR_ESG;
  }

protected:
  DMABaseClass() {}

  friend void DMA0_DMA16_DriverIRQHandler(void);

  static inline void copy_tcd(TCD_t *dst, const TCD_t *src)
  {
    dst->CSR &= ~DMA_TCD_CSR_DONE;
    const uint32_t *p = (const uint32_t *)src;
    uint32_t *      q = (uint32_t *)dst;
    uint32_t        t1, t2, t3, t4;
    t1   = *p++;
    t2   = *p++;
    t3   = *p++;
    t4   = *p++;
    *q++ = t1;
    *q++ = t2;
    *q++ = t3;
    *q++ = t4;
    t1   = *p++;
    t2   = *p++;
    t3   = *p++;
    t4   = *p++;
    *q++ = t1;
    *q++ = t2;
    *q++ = t3;
    *q++ = t4;
  }
};

class DMASetting : public DMABaseClass
{
public:
  DMASetting() { TCD = &tcddata; }
  DMASetting(const DMASetting &c)
  {
    TCD   = &tcddata;
    *this = c;
  }
  DMASetting(const DMABaseClass &c)
  {
    TCD   = &tcddata;
    *this = c;
  }
  DMASetting &operator=(const DMABaseClass &rhs)
  {
    copy_tcd(TCD, rhs.TCD);
    return *this;
  }

private:
  TCD_t tcddata __attribute__((aligned(32)));
};

class DMAChannel : public DMABaseClass
{
public:
  /*************************************************/
  /**    Channel Allocation                       **/
  /*************************************************/

  DMAChannel() { begin(); }
  DMAChannel(const DMAChannel &c)
  {
    TCD     = c.TCD;
    channel = c.channel;
  }
  DMAChannel(const DMASetting &c)
  {
    begin();
    copy_tcd(TCD, c.TCD);
  }
  DMAChannel(bool allocate)
  {
    if(allocate)
      begin();
  }
  DMAChannel &operator=(const DMAChannel &rhs)
  {
    if(channel != rhs.channel)
    {
      release();
      TCD     = rhs.TCD;
      channel = rhs.channel;
    }
    return *this;
  }
  DMAChannel &operator=(const DMASetting &rhs)
  {
    copy_tcd(TCD, rhs.TCD);
    return *this;
  }
  ~DMAChannel() { release(); }
  void begin(bool force_initialization = false);

private:
  void release(void);

public:
  /***************************************/
  /**    Triggering                     **/
  /***************************************/

  void triggerAtHardwareEvent(uint8_t source)
  {
    volatile uint32_t *mux = &DMAMUX_CHCFG0 + channel;

    *mux = 0;
    *mux = (source & 0x7F) | DMAMUX_CHCFG_ENBL_ZM;
  }

  void triggerAtTransfersOf(DMABaseClass &ch)
  {
    ch.TCD->BITER = (ch.TCD->BITER & ~DMA_TCD_BITER_ELINKYES_LINKCH_MASK) | DMA_TCD_BITER_ELINKYES_LINKCH(channel)
                    | DMA_TCD_BITER_ELINKYES_ELINK;
    ch.TCD->CITER = ch.TCD->BITER;
  }

  void triggerAtCompletionOf(DMABaseClass &ch)
  {
    ch.TCD->CSR = (ch.TCD->CSR & ~(DMA_TCD_CSR_MAJORLINKCH_MASK | DMA_TCD_CSR_DONE)) | DMA_TCD_CSR_MAJORLINKCH(channel)
                  | DMA_TCD_CSR_MAJORELINK;
  }

  void triggerContinuously(void)
  {
#if 0
		volatile uint8_t *mux = (volatile uint8_t *)&DMAMUX0_CHCFG0;
		mux[channel] = 0;
#if DMAMUX_NUM_SOURCE_ALWAYS >= DMA_NUM_CHANNELS
		mux[channel] = DMAMUX_SOURCE_ALWAYS0 + channel;
#else

		unsigned int i = DMAMUX_SOURCE_ALWAYS0;
		for (i = DMAMUX_SOURCE_ALWAYS0;
		  i < DMAMUX_SOURCE_ALWAYS0 + DMAMUX_NUM_SOURCE_ALWAYS; i++) {
			unsigned int ch;
			for (ch=0; ch < DMA_NUM_CHANNELS; ch++) {
				if (mux[ch] == i) break;
			}
			if (ch >= DMA_NUM_CHANNELS) {
				mux[channel] = (i | DMAMUX_ENABLE);
				return;
			}
		}
#endif
#endif
  }

  void triggerManual(void) { DMA_SSRT = channel; }

  /***************************************/
  /**    Interrupts                     **/
  /***************************************/

  void attachInterrupt(void (*isr)(void), uint8_t noteToMyself);

  void detachInterrupt(void) { __NVIC_DisableIRQ((IRQn_Type)(0 + channel)); }

  void clearInterrupt(void) { DMA_CINT = channel; }

  /***************************************/
  /**    Enable / Disable               **/
  /***************************************/

  void enable(void) { DMA_SERQ = channel; }
  void disable(void) { DMA_CERQ = channel; }

  /***************************************/
  /**    Status                         **/
  /***************************************/

  bool complete(void)
  {
    if(TCD->CSR & DMA_TCD_CSR_DONE)
      return true;
    return false;
  }
  void clearComplete(void) { DMA_CDNE = channel; }
  bool error(void)
  {
    if(DMA_ERR & (1 << channel))
      return true;
    return false;
  }
  void  clearError(void) { DMA_CERR = channel; }
  void *sourceAddress(void) { return (void *)(TCD->SADDR); }
  void *destinationAddress(void) { return (void *)(TCD->DADDR); }

  /***************************************/
  /**    Direct Hardware Access         **/
  /***************************************/

  uint8_t channel;
};

void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2);
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3);
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3, DMAChannel &ch4);

#endif

#endif
