#if !defined(__ARM_ARCH_7EM__)

#include "output_i2s.h"

#include <iostream>

sound_block_t * SoundOutputI2S::block_left_1st        = NULL;
sound_block_t * SoundOutputI2S::block_right_1st       = NULL;
sound_block_t * SoundOutputI2S::block_left_2nd        = NULL;
sound_block_t * SoundOutputI2S::block_right_2nd       = NULL;
uint16_t        SoundOutputI2S::block_left_offset     = 0;
uint16_t        SoundOutputI2S::block_right_offset    = 0;
bool            SoundOutputI2S::update_responsibility = false;
static uint32_t i2s_tx_buffer[SOUND_BLOCK_SAMPLES];

#define S162F_SCALE 3.0517578125e-05f /**< 1 / (2** 15) */

float s162f(int16_t x)
{
  return (float)x * S162F_SCALE;
}

void SoundOutputI2S::begin(void)
{
  block_left_1st  = NULL;
  block_right_1st = NULL;

  update_responsibility = update_setup();
}

volatile bool isFirstCall = true;

void SoundOutputI2S::isr(float buffer[])
{
  int16_t *      dest;
  sound_block_t *blockL, *blockR;
  uint32_t       saddr, offsetL, offsetR;

  if(isFirstCall)
  {
    dest = (int16_t *)&i2s_tx_buffer[SOUND_BLOCK_SAMPLES / 2];
    if(SoundOutputI2S::update_responsibility)
      SoundStream::update_all();
  }
  else
  {
    dest = (int16_t *)i2s_tx_buffer;
  }

  blockL  = SoundOutputI2S::block_left_1st;
  blockR  = SoundOutputI2S::block_right_1st;
  offsetL = SoundOutputI2S::block_left_offset;
  offsetR = SoundOutputI2S::block_right_offset;

  isFirstCall = !isFirstCall;

  if(blockL && blockR)
  {
    for(int i = 0; i < SOUND_BLOCK_SAMPLES / 2; i++)
    {
      buffer[i * 2]     = s162f(blockL->data[i + offsetL]);
      buffer[i * 2 + 1] = s162f(blockR->data[i + offsetR]);
    }

    offsetL += SOUND_BLOCK_SAMPLES / 2;
    offsetR += SOUND_BLOCK_SAMPLES / 2;
  }
  else if(blockL)
  {
    for(int i = 0; i < SOUND_BLOCK_SAMPLES / 2; i++)
    {
      buffer[i * 2]     = s162f(blockL->data[i + offsetL]);
      buffer[i * 2 + 1] = s162f((int16_t)0);
    }

    offsetL += SOUND_BLOCK_SAMPLES / 2;
  }
  else if(blockR)
  {
    for(int i = 0; i < SOUND_BLOCK_SAMPLES / 2; i++)
    {
      buffer[i * 2]     = s162f((int16_t)0);
      buffer[i * 2 + 1] = s162f(blockR->data[i + offsetR]);
    }

    offsetR += SOUND_BLOCK_SAMPLES / 2;
  }
  else
  {
    for(int i = 0; i < SOUND_BLOCK_SAMPLES; i++)
    {
      buffer[i] = s162f((int16_t)0);
    }
  }

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
  sound_block_t *block;
  block = receiveReadOnly(0);
  if(block)
  {
    if(block_left_1st == NULL)
    {
      block_left_1st    = block;
      block_left_offset = 0;
    }
    else if(block_left_2nd == NULL)
    {
      block_left_2nd = block;
    }
    else
    {
      sound_block_t *tmp = block_left_1st;
      block_left_1st     = block_left_2nd;
      block_left_2nd     = block;
      block_left_offset  = 0;

      release(tmp);
    }
  }
  block = receiveReadOnly(1);
  if(block)
  {
    if(block_right_1st == NULL)
    {
      block_right_1st    = block;
      block_right_offset = 0;
    }
    else if(block_right_2nd == NULL)
    {
      block_right_2nd = block;
    }
    else
    {
      sound_block_t *tmp = block_right_1st;
      block_right_1st    = block_right_2nd;
      block_right_2nd    = block;
      block_right_offset = 0;

      release(tmp);
    }
  }
}

#endif
