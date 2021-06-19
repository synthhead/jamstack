#ifndef play_memory_h_
#define play_memory_h_

#include "SoundStream.h"

class AudioPlayMemory : public SoundStream
{
public:
  AudioPlayMemory(void) : SoundStream(0, NULL), playing(0) {}
  void         play(const unsigned int* data);
  void         stop(void);
  bool         isPlaying(void) { return playing; }
  uint32_t     positionMillis(void);
  uint32_t     lengthMillis(void);
  virtual void update(void) override;

private:
  const unsigned int* next;
  const unsigned int* beginning;
  uint32_t            length;
  int16_t             prior;
  volatile uint8_t    playing;
};

#endif
