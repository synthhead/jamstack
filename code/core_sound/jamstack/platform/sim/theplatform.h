#ifndef JAMSTACK_THEPLATFORM
#define JAMSTACK_THEPLATFORM

#include <stdint.h>
#include <chrono>

auto start   = std::chrono::high_resolution_clock::now();
auto elapsed = std::chrono::high_resolution_clock::now() - start;

uint32_t micros(void)
{
  return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

#endif /* JAMSTACK_THEPLATFORM */
