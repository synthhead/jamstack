#include "effect_delay.h"

void SoundFxDelay::update(void)
{
  sound_block_t *output;
  uint32_t       head, tail, count, channel, index, prev, offset;
  const int16_t *src, *end;
  int16_t *      dst;

  head = headindex;
  tail = tailindex;
  if(++head >= DELAY_QUEUE_SIZE)
    head = 0;
  if(head == tail)
  {
    if(queue[tail] != NULL)
      release(queue[tail]);
    if(++tail >= DELAY_QUEUE_SIZE)
      tail = 0;
  }
  queue[head] = receiveReadOnly();
  headindex   = head;

  if(head >= tail)
  {
    count = head - tail;
  }
  else
  {
    count = DELAY_QUEUE_SIZE + head - tail;
  }
  if(count > maxblocks)
  {
    count -= maxblocks;
    do
    {
      if(queue[tail] != NULL)
      {
        release(queue[tail]);
        queue[tail] = NULL;
      }
      if(++tail >= DELAY_QUEUE_SIZE)
        tail = 0;
    } while(--count > 0);
  }
  tailindex = tail;

  for(channel = 0; channel < 8; channel++)
  {
    if(!(activemask & (1 << channel)))
      continue;
    index  = position[channel] / SOUND_BLOCK_SAMPLES;
    offset = position[channel] % SOUND_BLOCK_SAMPLES;
    if(head >= index)
    {
      index = head - index;
    }
    else
    {
      index = DELAY_QUEUE_SIZE + head - index;
    }
    if(offset == 0)
    {
      if(queue[index])
      {
        transmit(queue[index], channel);
      }
    }
    else
    {
      output = allocate();
      if(!output)
        continue;
      dst = output->data;
      if(index > 0)
      {
        prev = index - 1;
      }
      else
      {
        prev = DELAY_QUEUE_SIZE - 1;
      }
      if(queue[prev])
      {
        end = queue[prev]->data + SOUND_BLOCK_SAMPLES;
        src = end - offset;
        while(src < end)
        {
          *dst++ = *src++;
        }
      }
      else
      {
        end = dst + offset;
        while(dst < end)
        {
          *dst++ = 0;
        }
      }
      end = output->data + SOUND_BLOCK_SAMPLES;
      if(queue[index])
      {
        src = queue[index]->data;
        while(dst < end)
        {
          *dst++ = *src++;
        }
      }
      else
      {
        while(dst < end)
        {
          *dst++ = 0;
        }
      }
      transmit(output, channel);
      release(output);
    }
  }
}
