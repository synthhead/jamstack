#include "SoundStream.h"
#include "memspace.h"
#include <stdio.h>

#define MAX_SOUND_MEMORY 10000

#define NUM_MASKS (((MAX_SOUND_MEMORY / SOUND_BLOCK_SAMPLES / 2) + 31) / 32)

sound_block_t *SoundStream::memory_pool;
uint32_t       SoundStream::memory_pool_available_mask[NUM_MASKS];
uint16_t       SoundStream::memory_pool_first_mask;

uint16_t SoundStream::cpu_cycles_total     = 0;
uint16_t SoundStream::cpu_cycles_total_max = 0;
uint16_t SoundStream::memory_used          = 0;
uint16_t SoundStream::memory_used_max      = 0;

extern uint32_t g_audio_clock_updates;

void software_isr(void);

FLASHFUNC void SoundStream::initialize_memory(sound_block_t *data, unsigned int num)
{
  unsigned int i;
  unsigned int maxnum = MAX_SOUND_MEMORY / SOUND_BLOCK_SAMPLES / 2;

  if(num > maxnum)
    num = maxnum;
  __zm_disable_irq();
  memory_pool            = data;
  memory_pool_first_mask = 0;
  for(i = 0; i < NUM_MASKS; i++)
  {
    memory_pool_available_mask[i] = 0;
  }
  for(i = 0; i < num; i++)
  {
    memory_pool_available_mask[i >> 5] |= (1 << (i & 0x1F));
  }
  for(i = 0; i < num; i++)
  {
    data[i].memory_pool_index = i;
  }
  __zm_enable_irq();
}

sound_block_t *SoundStream::allocate(void)
{
  uint32_t       n, index, avail;
  uint32_t *     p, *end;
  sound_block_t *block;
  uint32_t       used;

  p   = memory_pool_available_mask;
  end = p + NUM_MASKS;
  __zm_disable_irq();
  index = memory_pool_first_mask;
  p += index;
  while(1)
  {
    if(p >= end)
    {
      __zm_enable_irq();

      return NULL;
    }
    avail = *p;
    if(avail)
      break;
    index++;
    p++;
  }
  n = __builtin_clz(avail);
  avail &= ~(0x80000000 >> n);
  *p = avail;
  if(!avail)
    index++;
  memory_pool_first_mask = index;
  used                   = memory_used + 1;
  memory_used            = used;
  __zm_enable_irq();
  index            = p - memory_pool_available_mask;
  block            = memory_pool + ((index << 5) + (31 - n));
  block->ref_count = 1;
  if(used > memory_used_max)
    memory_used_max = used;

  return block;
}

void SoundStream::release(sound_block_t *block)
{
  uint32_t mask  = (0x80000000 >> (31 - (block->memory_pool_index & 0x1F)));
  uint32_t index = block->memory_pool_index >> 5;

  __zm_disable_irq();
  if(block->ref_count > 1)
  {
    block->ref_count--;
  }
  else
  {
    memory_pool_available_mask[index] |= mask;
    if(index < memory_pool_first_mask)
      memory_pool_first_mask = index;
    memory_used--;
  }
  __zm_enable_irq();
}

void SoundStream::transmit(sound_block_t *block, unsigned char index)
{
  for(SoundConnection *c = destination_list; c != NULL; c = c->next_dest)
  {
    if(c->src_index == index)
    {
      if(c->dst.inputQueue[c->dest_index] == NULL)
      {
        c->dst.inputQueue[c->dest_index] = block;
        block->ref_count++;
      }
    }
  }
}

sound_block_t *SoundStream::receiveReadOnly(unsigned int index)
{
  sound_block_t *in;

  if(index >= num_inputs)
    return NULL;
  in                = inputQueue[index];
  inputQueue[index] = NULL;
  return in;
}

sound_block_t *SoundStream::receiveWritable(unsigned int index)
{
  sound_block_t *in, *p;

  if(index >= num_inputs)
    return NULL;
  in                = inputQueue[index];
  inputQueue[index] = NULL;
  if(in && in->ref_count > 1)
  {
    p = allocate();
    if(p)
      memcpy(p->data, in->data, sizeof(p->data));
    in->ref_count--;
    in = p;
  }
  return in;
}

void SoundConnection::connect(void)
{
  SoundConnection *p;

  if(isConnected)
    return;
  if(dest_index > dst.num_inputs)
    return;
  __zm_disable_irq();
  p = src.destination_list;
  if(p == NULL)
  {
    src.destination_list = this;
  }
  else
  {
    while(p->next_dest)
    {
      if(&p->src == &this->src && &p->dst == &this->dst && p->src_index == this->src_index
         && p->dest_index == this->dest_index)
      {
        __zm_enable_irq();
        return;
      }
      p = p->next_dest;
    }
    p->next_dest = this;
  }
  this->next_dest = NULL;
  src.numConnections++;
  src.active = true;

  dst.numConnections++;
  dst.active = true;

  isConnected = true;

  __zm_enable_irq();
}

void SoundConnection::disconnect(void)
{
  SoundConnection *p;

  if(!isConnected)
    return;
  if(dest_index > dst.num_inputs)
    return;
  __zm_disable_irq();

  p = src.destination_list;
  if(p == NULL)
  {
    __zm_enable_irq();
    return;
  }
  else if(p == this)
  {
    if(p->next_dest)
    {
      src.destination_list = next_dest;
    }
    else
    {
      src.destination_list = NULL;
    }
  }
  else
  {
    while(p)
    {
      if(p == this)
      {
        if(p->next_dest)
        {
          p = next_dest;
          break;
        }
        else
        {
          p = NULL;
          break;
        }
      }
      p = p->next_dest;
    }
  }

  if(dst.inputQueue[dest_index] != NULL)
  {
    SoundStream::release(dst.inputQueue[dest_index]);

    __zm_disable_irq();
    dst.inputQueue[dest_index] = NULL;
  }

  src.numConnections--;
  if(src.numConnections == 0)
  {
    src.active = false;
  }

  dst.numConnections--;
  if(dst.numConnections == 0)
  {
    dst.active = false;
  }

  isConnected = false;

  __zm_enable_irq();
}

bool SoundStream::update_scheduled = false;

bool SoundStream::update_setup(void)
{
  if(update_scheduled)
    return false;

  __NVIC_SetPriority(Reserved87_IRQn, 208);
  __NVIC_EnableIRQ(Reserved87_IRQn);

  update_scheduled = true;
  return true;
}

void SoundStream::update_stop(void)
{
  __NVIC_DisableIRQ(Reserved87_IRQn);
  update_scheduled = false;
}

SoundStream *SoundStream::first_update = NULL;

void Reserved87_IRQHandler(void)
{
  SoundStream *p;

  uint32_t totalcycles = ARM_DWT_CYCCNT;

  for(p = SoundStream::first_update; p; p = p->next_update)
  {
    if(p->active)
    {
      uint32_t cycles = ARM_DWT_CYCCNT;
      p->update();

      cycles        = (ARM_DWT_CYCCNT - cycles) >> 6;
      p->cpu_cycles = cycles;
      if(cycles > p->cpu_cycles_max)
        p->cpu_cycles_max = cycles;
    }
  }

  totalcycles                   = (ARM_DWT_CYCCNT - totalcycles) >> 6;
  SoundStream::cpu_cycles_total = totalcycles;
  if(totalcycles > SoundStream::cpu_cycles_total_max)
    SoundStream::cpu_cycles_total_max = totalcycles;

  g_audio_clock_updates++;

  asm("DSB");
}
