#ifndef JAMSTACK_COM_PATCHER
#define JAMSTACK_COM_PATCHER

#include "SoundStream.h"
#include "jamstackcommons.h"
#include "mixer.h"
#include "synth/synth_sine.h"

#include <map>
#include <memory>

namespace Jamstack
{
// typedef std::map<int64_t, std::unique_ptr<Class1>> ObjectArray;

class Patcher
{
public:
  Patcher()
  {
    //
    nodes_[0] = std::make_unique<SoundAmplifier>();
  }
  ~Patcher() {}

  enum NodeTypes
  {
    MIXER4,
    SYNTH_SINE,
    SYNTH_SINE_MODULATED,
    SYNTH_SINE_HIRES,
    SYNTH_WAVEFORM,
  };

  enum OpTypes
  {
    ADD,
    REMOVE,
    CONNECT,
    DISCONNECT,
    PARAM
  };

  SoundAmplifier *output() { return (SoundAmplifier *)(nodes_[0].get()); }

  void createNode(uint16_t nodeId, uint16_t nodeType);
  void removeNode(uint16_t nodeId);
  void connect(uint16_t cordId, uint16_t fromNodeId, uint8_t outlet, uint16_t toNodeId, uint8_t inlet);
  void disconnect(uint16_t cordId);
  void setParameter(uint16_t nodeId, uint8_t paramId, float value);
  void setParameter(uint16_t nodeId, uint8_t paramId, int value);

private:
  // std::map<int, SoundConnection *> cords_;
  // std::map<int, SoundStream *>     nodes_;

  std::map<int, std::unique_ptr<SoundConnection>> cords_;
  std::map<int, std::unique_ptr<SoundStream>>     nodes_;
};

} // namespace Jamstack

#endif /* JAMSTACK_COM_PATCHER */
