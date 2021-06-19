#ifndef JAMSTACK_SYNTH
#define JAMSTACK_SYNTH

#include "Synth.h"

Synth::Synth()
{
  basicSynth_ = std::make_unique<BasicSynth>();
  patcher_1_  = std::make_unique<Jamstack::Patcher>();
  output_0_   = std::make_unique<SoundMixer4>();
  output_1_   = std::make_unique<SoundMixer4>();

  output_0()->gain(0, 1.f);
  output_0()->gain(1, 1.f);
  output_1()->gain(0, 1.f);
  output_1()->gain(1, 1.f);

  cords_[0] = std::make_unique<SoundConnection>(*basicSynth_->output, *output_0_);
  cords_[1] = std::make_unique<SoundConnection>(*basicSynth_->output, *output_1_);
  cords_[2] = std::make_unique<SoundConnection>(*patcher_1_->output(), 0, *output_0_, 1);
  cords_[3] = std::make_unique<SoundConnection>(*patcher_1_->output(), 0, *output_1_, 1);
}

Synth::~Synth() {}

void Synth::onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  basicSynth_->onNoteOn(note, velocity);
}

void Synth::onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  basicSynth_->onNoteOff(note);
}

void Synth::onControlChange(uint8_t channel, uint8_t control, uint8_t data)
{
  basicSynth_->onControlChange(control, data);
}
void Synth::onProgramChange(uint8_t channel, uint8_t control, uint8_t data) {}
void Synth::onPitchbendChange(uint8_t channel, uint8_t control, uint8_t data) {}
void Synth::onSysex(const uint8_t *data, uint16_t length, bool last)
{
  char sysexData[length - 5];
  printf("midi identifier: %d %d %d\r\n", data[1], data[2], data[3]);
  memcpy(sysexData, data + 4, length - 5);
  deserializeJson(jsonBuffer, sysexData);
  processJson();
}

void Synth::processJson(void)
{
  // const char *pOp
  uint8_t pOp = jsonBuffer["op"];
  if(pOp == Jamstack::Patcher::OpTypes::ADD)
  {
    int16_t pNodeId   = jsonBuffer["id"];
    int16_t pNodeType = jsonBuffer["type"];

    printf("ADD | Op: %d, id: %d, type: %d \r\n", pOp, pNodeId, pNodeType);
    patcher_1_->createNode(pNodeId, pNodeType);
  }
  else if(pOp == Jamstack::Patcher::OpTypes::REMOVE)
  {
    uint16_t pNodeId = jsonBuffer["id"];

    printf("REMOVE | Op: %d, id: %d \r\n", pOp, pNodeId);
    patcher_1_->removeNode(pNodeId);
  }
  else if(pOp == Jamstack::Patcher::OpTypes::CONNECT)
  {
    uint16_t pCordId = jsonBuffer["id"];
    uint8_t  pFrom   = jsonBuffer["from"];
    uint8_t  pOut    = jsonBuffer["out"];
    uint8_t  pTo     = jsonBuffer["to"];
    uint8_t  pIn     = jsonBuffer["in"];

    printf("CONNECT | Op: %d, from: %d, out: %d, to: %d, in: %d \r\n", pOp, pFrom, pOut, pTo, pIn);
    patcher_1_->connect(pCordId, pFrom, pOut, pTo, pIn);
  }
  else if(pOp == Jamstack::Patcher::OpTypes::DISCONNECT)
  {
    uint16_t pCordId = jsonBuffer["id"];

    printf("DISCONNECT | Op: %d, id: %d \r\n", pOp, pCordId);
    patcher_1_->disconnect(pCordId);
  }

  //   const char *pFrom = jsonBuffer["from"];
  // int         pOut = jsonBuffer["out"];
  // const char *pTo  = jsonBuffer["to"];
  // int         pInl = jsonBuffer["inl"];

  // printf("JSON | Op: %s, from: %s, out: %d, to: %s, inl: %d \r\n", pOp, pFrom, pOut, pTo, pInl);
}

#endif /* JAMSTACK_SYNTH */
