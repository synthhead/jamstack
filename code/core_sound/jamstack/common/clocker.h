#ifndef JAMSTACK_CLOCKER_H_
#define JAMSTACK_CLOCKER_H_

#include "SoundStream.h"
// #include "fsl_debug_console.h"
#include "tempo.h"

extern volatile uint32_t clock_test_count;

namespace jamstack
{
//
class Clocker : public SoundStream
{
public:
  Clocker(void) : SoundStream(0, inputQueueArray) {}

  virtual void update(void) override
  {
    //
    // PRINTF("clock \r\n");
    clock_test_count++;
    tempo.RefreshInternalClock();
  }

private:
  sound_block_t *inputQueueArray[1];
};

} // namespace jamstack
#endif /* JAMSTACK_CLOCKER_H_ */
