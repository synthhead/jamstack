#include "com_patcher.h"

using namespace Jamstack;

void Patcher::createNode(uint16_t nodeId, uint16_t nodeType)
{
  if(nodes_.find(nodeId) != nodes_.end())
  {
    printf("Error: node with id \"%d\" already exists.\r\n", nodeType);
    return; // TODO: report error
  }

  switch(nodeType)
  {
    case NodeTypes::MIXER4:
      printf("Added mixer\r\n");
      nodes_[nodeId] = std::make_unique<SoundMixer4>();
      break;

    case NodeTypes::SYNTH_SINE:
      printf("Added sine\r\n");
      nodes_[nodeId] = std::make_unique<SoundSynthWaveformSine>();
      break;

    case NodeTypes::SYNTH_SINE_MODULATED:
      printf("Added sine modulated\r\n");
      nodes_[nodeId] = std::make_unique<SoundSynthWaveformSineModulated>();
      break;

    default: printf("Error: received type \"%d\" is unknown.\r\n", nodeType); break;
  }
}
void Patcher::removeNode(uint16_t nodeId)
{
  if(nodes_.find(nodeId) == nodes_.end())
  {
    printf("Error: can't remove \"%d\" - doesn't exist.\r\n", nodeId);
    return;
  }

  // delete nodes_[nodeId];
  // nodes_[nodeId] = nullptr;
  // nodes_.erase(nodeId);
  nodes_.erase(nodeId);

  printf("Removed node %d\r\n", nodeId);
}

void Patcher::connect(uint16_t cordId, uint16_t fromNodeId, uint8_t outlet, uint16_t toNodeId, uint8_t inlet)
{
  if(nodes_.find(fromNodeId) == nodes_.end())
  {
    printf("Patcher error: fromNodeId not found (%d)\r\n", fromNodeId);
    return;
  }
  else if(nodes_.find(toNodeId) == nodes_.end())
  {
    printf("Patcher error: toNodeId not found (%d)\r\n", toNodeId);
    return;
  }
  else if(cords_.find(cordId) != cords_.end())
  {
    printf("Patcher error: cordId already exists (%d)\r\n", cordId);
    return;
  }

  // TODO: check if nodes_ has specified number inlets/outlets

  cords_[cordId] = std::make_unique<SoundConnection>(*nodes_[fromNodeId], outlet, *nodes_[toNodeId], inlet);
  printf("connected %d\r\n", cordId);
}

void Patcher::disconnect(uint16_t cordId)
{
  if(cords_.find(cordId) == cords_.end())
  {
    printf("Error: cord with id \"%d\" not found.\r\n", cordId);
    return; // TODO: report error
  }

  // delete cords_[cordId];
  // cords_[cordId] = nullptr;
  // cords_.erase(cordId);
  cords_.erase(cordId);

  printf("Disconnected %d\r\n", cordId);
}

void Patcher::setParameter(uint16_t nodeId, uint8_t paramId, int value)
{
  // static_cast<SoundSynthWaveformSine *>(nodes_[0])->frequency(220.f);
  //  nodes_[nodeId]->parameter(paramId, value);
}
