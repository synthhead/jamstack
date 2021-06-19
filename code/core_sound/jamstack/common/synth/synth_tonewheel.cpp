#include "synth_tonewheel.h"
#include "utils/dspinst.h"

void SoundSynthHammond::update(void)
{
  sound_block_t *block;
  uint32_t i, ph1, ph2, ph3, ph4, ph5, ph6, ph7, ph8, ph9, inc1, inc2, inc3, inc4, inc5, inc6, inc7, inc8, inc9, index,
      scale;
  int32_t val1, val2;
  int32_t vt1, vt2, vt3, vt4, vt5, vt6, vt7, vt8, vt9;

  if(noteOn)
  {
    block = allocate();
    if(block)
    {
      ph1 = phase_accumulator1;
      if(percussiveVoice == 0)
      {
        ph2 = phase_accumulator2;
        ph3 = phase_accumulator3;
        ph4 = phase_accumulator4;
        ph5 = phase_accumulator5;
        ph6 = phase_accumulator6;
        ph7 = phase_accumulator7;
        ph8 = phase_accumulator8;
        ph9 = phase_accumulator9;
      }
      inc1 = phase_increment1;
      if(percussiveVoice == 0)
      {
        inc2 = phase_increment2;
        inc3 = phase_increment3;
        inc4 = phase_increment4;
        inc5 = phase_increment5;
        inc6 = phase_increment6;
        inc7 = phase_increment7;
        inc8 = phase_increment8;
        inc9 = phase_increment9;
      }
      for(i = 0; i < SOUND_BLOCK_SAMPLES; i++)
      {
        index = ph1 >> 24;
        val1  = toneGenRAM[index];
        val2  = toneGenRAM[index + 1];
        scale = (ph1 >> 8) & 0xFFFF;
        val2 *= scale;
        val1 *= 0x10000 - scale;
        vt1 = multiply_32x32_rshift32(val1 + val2, magnitude1);
        if(percussiveVoice == 0)
        {
          index = ph2 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph2 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt2   = multiply_32x32_rshift32(val1 + val2, magnitude2);
          index = ph3 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph3 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt3   = multiply_32x32_rshift32(val1 + val2, magnitude3);
          index = ph4 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph4 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt4   = multiply_32x32_rshift32(val1 + val2, magnitude4);
          index = ph5 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph5 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt5   = multiply_32x32_rshift32(val1 + val2, magnitude5);
          index = ph6 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph6 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt6   = multiply_32x32_rshift32(val1 + val2, magnitude6);
          index = ph7 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph7 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt7   = multiply_32x32_rshift32(val1 + val2, magnitude7);
          index = ph8 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph8 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt8   = multiply_32x32_rshift32(val1 + val2, magnitude8);
          index = ph9 >> 24;
          val1  = toneGenRAM[index];
          val2  = toneGenRAM[index + 1];
          scale = (ph9 >> 8) & 0xFFFF;
          val2 *= scale;
          val1 *= 0x10000 - scale;
          vt9 = multiply_32x32_rshift32(val1 + val2, magnitude9);
          vt1 = (vt1 + vt2 + vt3 + vt4 + vt5 + vt6 + vt7 + vt8 + vt9) >> 2;
        }

        block->data[i] = vt1;

        ph1 += inc1;
        ph2 += inc2;
        ph3 += inc3;
        ph4 += inc4;
        ph5 += inc5;
        ph6 += inc6;
        ph7 += inc7;
        ph8 += inc8;
        ph9 += inc9;
      }
      phase_accumulator1 = ph1;
      phase_accumulator2 = ph2;
      phase_accumulator3 = ph3;
      phase_accumulator4 = ph4;
      phase_accumulator5 = ph5;
      phase_accumulator6 = ph6;
      phase_accumulator7 = ph7;
      phase_accumulator8 = ph8;
      phase_accumulator9 = ph9;
      transmit(block);
      release(block);
      return;
    }
  }
}
