#include "play_memory.h"
#include "jamstackcommons.h"
#include "utils/dspinst.h"
#include "data/data_ulaw.h"

void AudioPlayMemory::play(const unsigned int *data)
{
  uint32_t format;

  playing   = 0;
  prior     = 0;
  format    = *data++;
  next      = data;
  beginning = data;
  length    = format & 0xFFFFFF;
  playing   = format >> 24;
}

void AudioPlayMemory::stop(void)
{
  playing = 0;
}

void AudioPlayMemory::update(void)
{
  sound_block_t *     block;
  const unsigned int *in;
  int16_t *           out;
  uint32_t            tmp32, consumed;
  int16_t             s0, s1, s2, s3, s4;
  int                 i;

  if(!playing)
    return;
  block = allocate();
  if(block == NULL)
    return;

  out = block->data;
  in  = next;
  s0  = prior;

  switch(playing)
  {
    case 0x01:
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i += 4)
      {
        tmp32  = *in++;
        *out++ = ulaw_decode_table[(tmp32 >> 0) & 255];
        *out++ = ulaw_decode_table[(tmp32 >> 8) & 255];
        *out++ = ulaw_decode_table[(tmp32 >> 16) & 255];
        *out++ = ulaw_decode_table[(tmp32 >> 24) & 255];
      }
      consumed = SOUND_BLOCK_SAMPLES;
      break;

    case 0x81:
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i += 2)
      {
        tmp32  = *in++;
        *out++ = (int16_t)(tmp32 & 65535);
        *out++ = (int16_t)(tmp32 >> 16);
      }
      consumed = SOUND_BLOCK_SAMPLES;
      break;

    case 0x02:
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i += 8)
      {
        tmp32  = *in++;
        s1     = ulaw_decode_table[(tmp32 >> 0) & 255];
        s2     = ulaw_decode_table[(tmp32 >> 8) & 255];
        s3     = ulaw_decode_table[(tmp32 >> 16) & 255];
        s4     = ulaw_decode_table[(tmp32 >> 24) & 255];
        *out++ = (s0 + s1) >> 1;
        *out++ = s1;
        *out++ = (s1 + s2) >> 1;
        *out++ = s2;
        *out++ = (s2 + s3) >> 1;
        *out++ = s3;
        *out++ = (s3 + s4) >> 1;
        *out++ = s4;
        s0     = s4;
      }
      consumed = SOUND_BLOCK_SAMPLES / 2;
      break;

    case 0x82:
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i += 4)
      {
        tmp32  = *in++;
        s1     = (int16_t)(tmp32 & 65535);
        s2     = (int16_t)(tmp32 >> 16);
        *out++ = (s0 + s1) >> 1;
        *out++ = s1;
        *out++ = (s1 + s2) >> 1;
        *out++ = s2;
        s0     = s2;
      }
      consumed = SOUND_BLOCK_SAMPLES / 2;
      break;

    case 0x03:
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i += 16)
      {
        tmp32  = *in++;
        s1     = ulaw_decode_table[(tmp32 >> 0) & 255];
        s2     = ulaw_decode_table[(tmp32 >> 8) & 255];
        s3     = ulaw_decode_table[(tmp32 >> 16) & 255];
        s4     = ulaw_decode_table[(tmp32 >> 24) & 255];
        *out++ = (s0 * 3 + s1) >> 2;
        *out++ = (s0 + s1) >> 1;
        *out++ = (s0 + s1 * 3) >> 2;
        *out++ = s1;
        *out++ = (s1 * 3 + s2) >> 2;
        *out++ = (s1 + s2) >> 1;
        *out++ = (s1 + s2 * 3) >> 2;
        *out++ = s2;
        *out++ = (s2 * 3 + s3) >> 2;
        *out++ = (s2 + s3) >> 1;
        *out++ = (s2 + s3 * 3) >> 2;
        *out++ = s3;
        *out++ = (s3 * 3 + s4) >> 2;
        *out++ = (s3 + s4) >> 1;
        *out++ = (s3 + s4 * 3) >> 2;
        *out++ = s4;
        s0     = s4;
      }
      consumed = SOUND_BLOCK_SAMPLES / 4;
      break;

    case 0x83:
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i += 8)
      {
        tmp32  = *in++;
        s1     = (int16_t)(tmp32 & 65535);
        s2     = (int16_t)(tmp32 >> 16);
        *out++ = (s0 * 3 + s1) >> 2;
        *out++ = (s0 + s1) >> 1;
        *out++ = (s0 + s1 * 3) >> 2;
        *out++ = s1;
        *out++ = (s1 * 3 + s2) >> 2;
        *out++ = (s1 + s2) >> 1;
        *out++ = (s1 + s2 * 3) >> 2;
        *out++ = s2;
        s0     = s2;
      }
      consumed = SOUND_BLOCK_SAMPLES / 4;
      break;

    default:
      release(block);
      playing = 0;
      return;
  }
  prior = s0;
  next  = in;
  if(length > consumed)
  {
    length -= consumed;
  }
  else
  {
    playing = 0;
  }
  transmit(block);
  release(block);
}

#define B2M_88200 (uint32_t)((double)4294967296000.0 / SOUND_SAMPLING_RATE_EXACT / 2.0)
#define B2M_44100 (uint32_t)((double)4294967296000.0 / SOUND_SAMPLING_RATE_EXACT)
#define B2M_22050 (uint32_t)((double)4294967296000.0 / SOUND_SAMPLING_RATE_EXACT * 2.0)
#define B2M_11025 (uint32_t)((double)4294967296000.0 / SOUND_SAMPLING_RATE_EXACT * 4.0)

uint32_t AudioPlayMemory::positionMillis(void)
{
  uint8_t        p;
  const uint8_t *n, *b;
  uint32_t       b2m;

  IrqAllDisable();
  p = playing;
  n = (const uint8_t *)next;
  b = (const uint8_t *)beginning;
  IrqAllEnable();
  switch(p)
  {
    case 0x81: b2m = B2M_88200; break;
    case 0x01:
    case 0x82: b2m = B2M_44100; break;
    case 0x02:
    case 0x83: b2m = B2M_22050; break;
    case 0x03: b2m = B2M_11025; break;
    default: return 0;
  }
  if(p == 0)
    return 0;
  return ((uint64_t)(n - b) * b2m) >> 32;
}

uint32_t AudioPlayMemory::lengthMillis(void)
{
  uint8_t         p;
  const uint32_t *b;
  uint32_t        b2m;

  IrqAllDisable();
  p = playing;
  b = (const uint32_t *)beginning;
  IrqAllEnable();
  switch(p)
  {
    case 0x81:
    case 0x01: b2m = B2M_44100; break;
    case 0x82:
    case 0x02: b2m = B2M_22050; break;
    case 0x83:
    case 0x03: b2m = B2M_11025; break;
    default: return 0;
  }
  return ((uint64_t)(*(b - 1) & 0xFFFFFF) * b2m) >> 32;
}
