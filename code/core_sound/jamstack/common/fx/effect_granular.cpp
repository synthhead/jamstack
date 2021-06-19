#include "effect_granular.h"

void SoundFxGranular::begin(int16_t *sample_bank_def, int16_t max_len_def)
{
  max_sample_len   = max_len_def;
  grain_mode       = 0;
  read_head        = 0;
  write_head       = 0;
  prev_input       = 0;
  playpack_rate    = 65536;
  accumulator      = 0;
  allow_len_change = true;
  sample_loaded    = false;
  sample_bank      = sample_bank_def;
}

void SoundFxGranular::beginFreeze_int(int grain_samples)
{
  IrqAllDisable();
  grain_mode = 1;
  if(grain_samples < max_sample_len)
  {
    freeze_len = grain_samples;
  }
  else
  {
    freeze_len = grain_samples;
  }
  sample_loaded = false;
  write_en      = false;
  sample_req    = true;
  IrqAllEnable();
}

void SoundFxGranular::beginPitchShift_int(int grain_samples)
{
  IrqAllDisable();
  grain_mode = 2;
  if(allow_len_change)
  {
    if(grain_samples < 100)
      grain_samples = 100;
    int maximum = (max_sample_len - 1) / 3;
    if(grain_samples > maximum)
      grain_samples = maximum;
    glitch_len = grain_samples;
  }
  sample_loaded = false;
  write_en      = false;
  sample_req    = true;
  IrqAllEnable();
}

void SoundFxGranular::stop()
{
  grain_mode       = 0;
  allow_len_change = true;
}

void SoundFxGranular::update(void)
{
  sound_block_t *block;

  if(sample_bank == NULL)
  {
    block = receiveReadOnly(0);
    if(block)
      release(block);
    return;
  }

  block = receiveWritable(0);
  if(!block)
    return;

  if(grain_mode == 0)
  {
    prev_input = block->data[SOUND_BLOCK_SAMPLES - 1];
  }
  else if(grain_mode == 1)
  {
    for(int j = 0; j < SOUND_BLOCK_SAMPLES; j++)
    {
      if(sample_req)
      {
        int16_t current_input = block->data[j];
        if((current_input < 0 && prev_input >= 0) || (current_input >= 0 && prev_input < 0))
        {
          write_en   = true;
          write_head = 0;
          read_head  = 0;
          sample_req = false;
        }
        else
        {
          prev_input = current_input;
        }
      }
      if(write_en)
      {
        sample_bank[write_head++] = block->data[j];
        if(write_head >= freeze_len)
        {
          sample_loaded = true;
        }
        if(write_head >= max_sample_len)
        {
          write_en = false;
        }
      }
      if(sample_loaded)
      {
        if(playpack_rate >= 0)
        {
          accumulator += playpack_rate;
          read_head = accumulator >> 16;
        }
        if(read_head >= freeze_len)
        {
          accumulator = 0;
          read_head   = 0;
        }
        block->data[j] = sample_bank[read_head];
      }
    }
  }
  else if(grain_mode == 2)
  {
    for(int k = 0; k < SOUND_BLOCK_SAMPLES; k++)
    {
      if(sample_req)
      {
        int16_t current_input = block->data[k];
        if((current_input < 0 && prev_input >= 0) || (current_input >= 0 && prev_input < 0))
        {
          write_en = true;
        }
        else
        {
          prev_input = current_input;
        }
      }

      if(write_en)
      {
        sample_req       = false;
        allow_len_change = true;

        if(write_head >= glitch_len)
        {
          write_head       = 0;
          sample_loaded    = true;
          write_en         = false;
          allow_len_change = false;
        }
        sample_bank[write_head] = block->data[k];
        write_head++;
      }

      if(sample_loaded)
      {
        float   fade_len = 20.00;
        int16_t m2       = fade_len;

        for(int m = 0; m < 2; m++)
        {
          sample_bank[m + glitch_len] = 0;
        }

        for(int m = 2; m < glitch_len - m2; m++)
        {
          sample_bank[m + glitch_len] = sample_bank[m];
        }

        for(int m = glitch_len - m2; m < glitch_len; m++)
        {
          float fadet                 = sample_bank[m] * (m2 / fade_len);
          sample_bank[m + glitch_len] = (int16_t)fadet;
          m2--;
        }
        sample_loaded = false;
        prev_input    = block->data[k];
        sample_req    = true;
      }

      accumulator += playpack_rate;
      read_head = (accumulator >> 16);

      if(read_head >= glitch_len)
      {
        read_head -= glitch_len;
        accumulator = 0;

        for(int m = 0; m < glitch_len; m++)
        {
          sample_bank[m + (glitch_len * 2)] = sample_bank[m + glitch_len];
        }
      }
      block->data[k] = sample_bank[read_head + (glitch_len * 2)];
    }
  }
  transmit(block);
  release(block);
}
