#include "effect_line.h"

#define STATE_IDLE    0
#define STATE_DELAY   1
#define STATE_ATTACK  2
#define STATE_HOLD    3
#define STATE_DECAY   4
#define STATE_SUSTAIN 5
#define STATE_RELEASE 6
#define STATE_FORCED  7

void SoundFxLine::noteOn(void)
{
  IrqAllDisable();
  if(state == STATE_IDLE || state == STATE_DELAY || release_forced_count == 0)
  {
    mult_hires = 0;
    count      = delay_count;
    if(count > 0)
    {
      state     = STATE_DELAY;
      inc_hires = 0;
    }
    else
    {
      state     = STATE_ATTACK;
      count     = attack_count;
      inc_hires = 0x40000000 / (int32_t)count;
    }
  }
  else if(state != STATE_FORCED)
  {
    state     = STATE_FORCED;
    count     = release_forced_count;
    inc_hires = (-mult_hires) / (int32_t)count;
  }
  IrqAllEnable();
}

void SoundFxLine::noteOff(void)
{
  IrqAllDisable();
  if(state != STATE_IDLE && state != STATE_FORCED)
  {
    state     = STATE_RELEASE;
    count     = release_count;
    inc_hires = (-mult_hires) / (int32_t)count;
  }
  IrqAllEnable();
}

void SoundFxLine::update(void)
{
  sound_block_t *block;
  uint32_t *     p, *end;
  uint32_t       sample12, sample34, sample56, sample78, tmp1, tmp2;

  block = receiveWritable();
  if(!block)
    return;
  if(state == STATE_IDLE)
  {
    release(block);
    return;
  }
  p   = (uint32_t *)(block->data);
  end = p + SOUND_BLOCK_SAMPLES / 2;

  while(p < end)
  {
    if(count == 0)
    {
      if(state == STATE_ATTACK)
      {
        count = hold_count;
        if(count > 0)
        {
          state      = STATE_HOLD;
          mult_hires = 0x40000000;
          inc_hires  = 0;
        }
        else
        {
          state     = STATE_DECAY;
          count     = decay_count;
          inc_hires = (sustain_mult - 0x40000000) / (int32_t)count;
        }
        continue;
      }
      else if(state == STATE_HOLD)
      {
        state     = STATE_DECAY;
        count     = decay_count;
        inc_hires = (sustain_mult - 0x40000000) / (int32_t)count;
        continue;
      }
      else if(state == STATE_DECAY)
      {
        state      = STATE_SUSTAIN;
        count      = 0xFFFF;
        mult_hires = sustain_mult;
        inc_hires  = 0;
      }
      else if(state == STATE_SUSTAIN)
      {
        count = 0xFFFF;
      }
      else if(state == STATE_RELEASE)
      {
        state = STATE_IDLE;
        while(p < end)
        {
          *p++ = 0;
          *p++ = 0;
          *p++ = 0;
          *p++ = 0;
        }
        break;
      }
      else if(state == STATE_FORCED)
      {
        mult_hires = 0;
        count      = delay_count;
        if(count > 0)
        {
          state     = STATE_DELAY;
          inc_hires = 0;
        }
        else
        {
          state     = STATE_ATTACK;
          count     = attack_count;
          inc_hires = 0x40000000 / (int32_t)count;
        }
      }
      else if(state == STATE_DELAY)
      {
        state     = STATE_ATTACK;
        count     = attack_count;
        inc_hires = 0x40000000 / count;
        continue;
      }
    }

    int32_t mult = mult_hires >> 14;
    int32_t inc  = inc_hires >> 17;

    sample12 = *p++;
    sample34 = *p++;
    sample56 = *p++;
    sample78 = *p++;
    p -= 4;
    mult += inc;
    tmp1 = signed_multiply_32x16b(mult, sample12);
    mult += inc;
    tmp2     = signed_multiply_32x16t(mult, sample12);
    sample12 = pack_16b_16b(tmp2, tmp1);
    mult += inc;
    tmp1 = signed_multiply_32x16b(mult, sample34);
    mult += inc;
    tmp2     = signed_multiply_32x16t(mult, sample34);
    sample34 = pack_16b_16b(tmp2, tmp1);
    mult += inc;
    tmp1 = signed_multiply_32x16b(mult, sample56);
    mult += inc;
    tmp2     = signed_multiply_32x16t(mult, sample56);
    sample56 = pack_16b_16b(tmp2, tmp1);
    mult += inc;
    tmp1 = signed_multiply_32x16b(mult, sample78);
    mult += inc;
    tmp2     = signed_multiply_32x16t(mult, sample78);
    sample78 = pack_16b_16b(tmp2, tmp1);
    *p++     = sample12;
    *p++     = sample34;
    *p++     = sample56;
    *p++     = sample78;

    mult_hires += inc_hires;
    count--;
  }
  transmit(block);
  release(block);
}

bool SoundFxLine::isActive()
{
  uint8_t current_state = *(volatile uint8_t *)&state;
  if(current_state == STATE_IDLE)
    return false;
  return true;
}

bool SoundFxLine::isSustain()
{
  uint8_t current_state = *(volatile uint8_t *)&state;
  if(current_state == STATE_SUSTAIN)
    return true;
  return false;
}
