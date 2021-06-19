#ifndef SoundStream_h
#define SoundStream_h

#ifndef __ASSEMBLER__

#include "MIMXRT1021.h"
#include "memspace.h"

#include <micro1020.h>
#include <stdio.h>
#include <string.h>

#endif

#ifndef SOUND_BLOCK_SAMPLES
#define SOUND_BLOCK_SAMPLES 128
#endif

#ifndef SOUND_SAMPLING_RATE_EXACT
#define SOUND_SAMPLING_RATE_EXACT 44100.0f
#endif

#define SOUND_SAMPLING_RATE SOUND_SAMPLING_RATE_EXACT

#ifndef __ASSEMBLER__

#ifdef __cplusplus
extern "C"
{
#endif
  extern void Reserved87_IRQHandler(void);
#ifdef __cplusplus
}
#endif

class SoundStream;
class SoundConnection;

typedef struct audio_block_struct
{
  uint8_t  ref_count;
  uint8_t  reserved1;
  uint16_t memory_pool_index;
  int16_t  data[SOUND_BLOCK_SAMPLES];
} sound_block_t;

class SoundConnection
{
public:
  SoundConnection(SoundStream &source, SoundStream &destination)
  : src(source), dst(destination), src_index(0), dest_index(0), next_dest(NULL)
  {
    isConnected = false;
    connect();
  }
  SoundConnection(SoundStream & source,
                  unsigned char sourceOutput,
                  SoundStream & destination,
                  unsigned char destinationInput)
  : src(source), dst(destination), src_index(sourceOutput), dest_index(destinationInput), next_dest(NULL)
  {
    isConnected = false;
    connect();
  }
  friend class SoundStream;
  ~SoundConnection() { disconnect(); }
  void disconnect(void);
  void connect(void);

protected:
  SoundStream &    src;
  SoundStream &    dst;
  unsigned char    src_index;
  unsigned char    dest_index;
  SoundConnection *next_dest;
  bool             isConnected;
};

#define SoundRam(num)                              \
  (                                                \
      {                                            \
        static DMAMEM sound_block_t data[num];     \
        SoundStream::initialize_memory(data, num); \
      })

#define CYCLE_COUNTER_APPROX_PERCENT(n)                                           \
  (((n) + (F_CPU_ACTUAL / 128 / SOUND_SAMPLING_RATE * SOUND_BLOCK_SAMPLES / 100)) \
   / (F_CPU_ACTUAL / 64 / SOUND_SAMPLING_RATE * SOUND_BLOCK_SAMPLES / 100))

#define SoundCpuUsage()         (CYCLE_COUNTER_APPROX_PERCENT(SoundStream::cpu_cycles_total))
#define SoundCpuUsageMax()      (CYCLE_COUNTER_APPROX_PERCENT(SoundStream::cpu_cycles_total_max))
#define SoundCpuUsageMaxReset() (SoundStream::cpu_cycles_total_max = SoundStream::cpu_cycles_total)
#define SoundRamUsage()         (SoundStream::memory_used)
#define SoundRamUsageMax()      (SoundStream::memory_used_max)
#define SoundRamUsageMaxReset() (SoundStream::memory_used_max = SoundStream::memory_used)

class SoundStream
{
public:
  SoundStream(unsigned char ninput, sound_block_t **iqueue) : num_inputs(ninput), inputQueue(iqueue)
  {
    active           = false;
    destination_list = NULL;
    for(int i = 0; i < num_inputs; i++)
    {
      inputQueue[i] = NULL;
    }

    if(first_update == NULL)
    {
      first_update = this;
    }
    else
    {
      SoundStream *p;
      for(p = first_update; p->next_update; p = p->next_update)
        ;
      p->next_update = this;
    }
    next_update    = NULL;
    cpu_cycles     = 0;
    cpu_cycles_max = 0;
    numConnections = 0;
  }
  virtual ~SoundStream() {}

  static void     initialize_memory(sound_block_t *data, unsigned int num);
  int             processorUsage(void) { return CYCLE_COUNTER_APPROX_PERCENT(cpu_cycles); }
  int             processorUsageMax(void) { return CYCLE_COUNTER_APPROX_PERCENT(cpu_cycles_max); }
  void            processorUsageMaxReset(void) { cpu_cycles_max = cpu_cycles; }
  bool            isActive(void) { return active; }
  uint16_t        cpu_cycles;
  uint16_t        cpu_cycles_max;
  static uint16_t cpu_cycles_total;
  static uint16_t cpu_cycles_total_max;
  static uint16_t memory_used;
  static uint16_t memory_used_max;

protected:
  bool                  active;
  unsigned char         num_inputs;
  static sound_block_t *allocate(void);
  static void           release(sound_block_t *block);
  void                  transmit(sound_block_t *block, unsigned char index = 0);
  sound_block_t *       receiveReadOnly(unsigned int index = 0);
  sound_block_t *       receiveWritable(unsigned int index = 0);
  static bool           update_setup(void);
  static void           update_stop(void);
  static void           update_all(void) { __NVIC_SetPendingIRQ(Reserved87_IRQn); }
  friend void           software_isr(void);
  friend void           Reserved87_IRQHandler(void);

  friend class SoundConnection;
  uint8_t numConnections;

private:
  SoundConnection *     destination_list;
  sound_block_t **      inputQueue;
  static bool           update_scheduled;
  virtual void          update(void) = 0;
  static SoundStream *  first_update;
  SoundStream *         next_update;
  static sound_block_t *memory_pool;
  static uint32_t       memory_pool_available_mask[];
  static uint16_t       memory_pool_first_mask;
};

#endif
#endif
