#pragma once
#ifndef jamstackcommons_h
#define jamstackcommons_h

#ifndef __ASSEMBLER__

#include <stdint.h>
#include <stdio.h>  // for NULL
#include <string.h> // for memcpy
// #include "fsl_debug_console.h" // bad for emulator

#endif // #ifndef __ASSEMBLER__

// #ifdef __cplusplus // do we really need this??

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName &);              \
  void operator=(const TypeName &)

#if defined(__ARM_ARCH_7EM__)

#define IrqAllDisable() (__disable_irq())
#define IrqAllEnable()  (__enable_irq())

#else

#define IrqAllDisable()
#define IrqAllEnable()

#endif // defined(__ARM_ARCH_7EM__)

#include "mathdefs.h" // PI, HALF_PI, etc.

namespace
{
static uint32_t seed;
}

// RANDOM
namespace jamstack
{
static void randomSeed(uint32_t newseed)
{
  if(newseed > 0)
    seed = newseed;
}

static void srandom(unsigned int newseed)
{
  seed = newseed;
}

static int32_t random(void)
{
  int32_t hi, lo, x;

  // the algorithm used in avr-libc 1.6.4
  x = seed;
  if(x == 0)
    x = 123459876;
  hi = x / 127773;
  lo = x % 127773;
  x  = 16807 * lo - 2836 * hi;
  if(x < 0)
    x += 0x7FFFFFFF;
  seed = x;
  return x;
}

static uint32_t random(uint32_t howbig)
{
  if(howbig == 0)
    return 0;
  return random() % howbig;
}

static int32_t random(int32_t howsmall, int32_t howbig)
{
  if(howsmall >= howbig)
    return howsmall;
  int32_t diff = howbig - howsmall;
  return random(diff) + howsmall;
}
} // namespace jamstack

// #endif // #ifdef __cplusplus
#endif // #define jamstackcommons_h
