

#include "synth_noisepink.h"

int16_t SoundSynthNoisePink::instance_cnt = 0;

#define Fn(cf, m, shift) (2048 * cf * (2 * ((m) >> shift & 1) - 1))
#define FA(n)                                                                                                       \
  (int32_t)(Fn(1.190566, n, 0) + Fn(0.162580, n, 1) + Fn(0.002208, n, 2) + Fn(0.025475, n, 3) + Fn(-0.001522, n, 4) \
            + Fn(0.007322, n, 5))
#define FB(n)                                                                                                        \
  (int32_t)(Fn(0.001774, n, 0) + Fn(0.004529, n, 1) + Fn(-0.001561, n, 2) + Fn(0.000776, n, 3) + Fn(-0.000486, n, 4) \
            + Fn(0.002017, n, 5))
#define FA8(n) FA(n), FA(n + 1), FA(n + 2), FA(n + 3), FA(n + 4), FA(n + 5), FA(n + 6), FA(n + 7)
#define FB8(n) FB(n), FB(n + 1), FB(n + 2), FB(n + 3), FB(n + 4), FB(n + 5), FB(n + 6), FB(n + 7)
const int32_t SoundSynthNoisePink::pfira[64] = {FA8(0), FA8(8), FA8(16), FA8(24), FA8(32), FA8(40), FA8(48), FA8(56)};
const int32_t SoundSynthNoisePink::pfirb[64] = {FB8(0), FB8(8), FB8(16), FB8(24), FB8(32), FB8(40), FB8(48), FB8(56)};

#define PM16(n) n, 0x80, 0x40, 0x80, 0x20, 0x80, 0x40, 0x80, 0x10, 0x80, 0x40, 0x80, 0x20, 0x80, 0x40, 0x80
const uint8_t SoundSynthNoisePink::pnmask[256] = {PM16(0),
                                                  PM16(8),
                                                  PM16(4),
                                                  PM16(8),
                                                  PM16(2),
                                                  PM16(8),
                                                  PM16(4),
                                                  PM16(8),
                                                  PM16(1),
                                                  PM16(8),
                                                  PM16(4),
                                                  PM16(8),
                                                  PM16(2),
                                                  PM16(8),
                                                  PM16(4),
                                                  PM16(8)};

#define PINT(bitmask, out) \
  bit = lfsr >> 31;        \
  dec &= ~bitmask;         \
  lfsr <<= 1;              \
  dec |= inc & bitmask;    \
  inc ^= bit & bitmask;    \
  accu += inc - dec;       \
  lfsr ^= bit & taps;      \
  out = accu + pfira[lfsr & 0x3F] + pfirb[lfsr >> 6 & 0x3F]

void SoundSynthNoisePink::update(void)
{
  sound_block_t *block;
  uint32_t *     p, *end;
  int32_t        n1, n2;
  int32_t        gain;
  int32_t        inc, dec, accu, bit, lfsr;
  int32_t        taps;

  gain = level;
  if(gain == 0)
    return;
  block = allocate();
  if(!block)
    return;
  p    = (uint32_t *)(block->data);
  end  = p + SOUND_BLOCK_SAMPLES / 2;
  taps = 0x46000001;
  inc  = pinc;
  dec  = pdec;
  accu = paccu;
  lfsr = plfsr;
  do
  {
    int32_t mask = pnmask[pncnt++];
    PINT(mask, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
    PINT(0x0400, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
    PINT(0x0200, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
    PINT(0x0400, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
    PINT(0x0100, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
    PINT(0x0400, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
    PINT(0x0200, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
    PINT(0x0400, n1);
    n1 = signed_multiply_32x16b(gain, n1);
    PINT(0x0800, n2);
    n2   = signed_multiply_32x16b(gain, n2);
    *p++ = pack_16b_16b(n2, n1);
  } while(p < end);
  pinc  = inc;
  pdec  = dec;
  paccu = accu;
  plfsr = lfsr;
  transmit(block);
  release(block);
}
