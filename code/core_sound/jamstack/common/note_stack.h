// Stack of currently pressed keys.
//
// Currently pressed keys are stored as a linked list. The linked list is used
// as a LIFO stack to allow monosynth-like behaviour. An example of such
// behaviour is:
// player presses and holds C4-> C4 is played.
// player presses and holds C5 (while holding C4) -> C5 is played.
// player presses and holds G4 (while holding C4&C5)-> G4 is played.
// player releases C5 -> G4 is played.
// player releases G4 -> C4 is played.
//
// The nodes used in the linked list are pre-allocated from a pool of N
// nodes, so the "pointers" (to the root element for example) are not actual
// pointers, but indices of an element in the pool.
//
// Additionally, an array of pointers is stored to allow random access to the
// n-th note, sorted by ascending order of pitch (for arpeggiation).

#ifndef JAMSTACK_NOTE_STACK_H_
#define JAMSTACK_NOTE_STACK_H_

#include "jamstackcommons.h"
// #include <cstring>

namespace jamstack
{
//
enum class NotePriority
{
  LAST,
  LOW,
  HIGH,
  FIRST,
};

static const uint8_t kFreeSlot = 0xFF;

struct NoteEntry
{
  uint8_t note;
  uint8_t velocity;
  uint8_t next_idx; // Base 1.
};

template <uint8_t capacity>
class NoteStack
{
public:
  NoteStack() {}
  ~NoteStack() {}

  void Init() { Clear(); }

  void NoteOn(uint8_t note, uint8_t velocity)
  {
    // Remove the note from the list first (in case it is already here).
    NoteOff(note);
    // In case of saturation, remove the least recently played note from the
    // stack.

    if(size_ == capacity)
    {
      uint8_t least_recent_note = 1;
      for(uint8_t i = 1; i <= capacity; ++i)
      {
        if(pool_[i].next_idx == 0)
        {
          least_recent_note = pool_[i].note;
        }
      }
      NoteOff(least_recent_note);
    }

    // Now we are ready to insert the new note. Find a free slot to insert it.
    uint8_t free_slot = 1;
    for(uint8_t i = 1; i <= capacity; ++i)
    {
      if(pool_[i].note == kFreeSlot)
      {
        free_slot = i;
        break;
      }
    }
    pool_[free_slot].next_idx = root_idx_;
    pool_[free_slot].note     = note;
    pool_[free_slot].velocity = velocity;
    root_idx_                 = free_slot;

    // The last step consists in inserting the note in the sorted list.
    for(uint8_t i = 0; i < size_; ++i)
    {
      if(pool_[sorted_idx_[i]].note > note)
      {
        for(uint8_t j = size_; j > i; --j)
        {
          sorted_idx_[j] = sorted_idx_[j - 1];
        }
        sorted_idx_[i] = free_slot;
        free_slot      = 0;
        break;
      }
    }
    if(free_slot)
    {
      sorted_idx_[size_] = free_slot;
    }
    ++size_;
  }

  void NoteOff(uint8_t note)
  {
    uint8_t current  = root_idx_;
    uint8_t previous = 0;

    while(current)
    {
      if(pool_[current].note == note)
      {
        break;
      }
      previous = current;
      current  = pool_[current].next_idx;
    }

    if(current)
    {
      if(previous)
      {
        pool_[previous].next_idx = pool_[current].next_idx;
      }
      else
      {
        root_idx_ = pool_[current].next_idx;
      }
      for(uint8_t i = 0; i < size_; ++i)
      {
        if(sorted_idx_[i] == current)
        {
          for(uint8_t j = i; j < size_ - 1; ++j)
          {
            sorted_idx_[j] = sorted_idx_[j + 1];
          }
          break;
        }
      }
      pool_[current].next_idx = 0;
      pool_[current].note     = kFreeSlot;
      pool_[current].velocity = 0;
      --size_;
    }
  }

  void Clear()
  {
    size_ = 0;
    memset(pool_ + 1, 0, sizeof(NoteEntry) * capacity);
    memset(sorted_idx_ + 1, 0, capacity);
    root_idx_ = 0;
    for(uint8_t i = 0; i <= capacity; ++i)
    {
      pool_[i].note = kFreeSlot;
    }
  }

  uint8_t          size() const { return size_; }
  uint8_t          max_size() const { return capacity; }
  const NoteEntry &most_recent_note() const { return pool_[root_idx_]; }
  const NoteEntry &least_recent_note() const
  {
    uint8_t current = root_idx_;
    while(current && pool_[current].next_idx)
    {
      current = pool_[current].next_idx;
    }
    return pool_[current];
  }
  const NoteEntry &played_note(uint8_t idx) const
  {
    uint8_t current = root_idx_;
    idx             = size_ - idx - 1;

    for(uint8_t i = 0; i < idx; ++i)
    {
      current = pool_[current].next_idx;
    }
    return pool_[current];
  }
  const NoteEntry &sorted_note(uint8_t idx) const { return pool_[sorted_idx_[index]]; }
  const NoteEntry &note(uint8_t index) const { return pool_[index]; }
  NoteEntry *      mutable_note(uint8_t index) { return &pool_[index]; }
  const NoteEntry &dummy() const { return pool_[0]; }
  const NoteEntry &note_by_priority(NotePriority priority, uint8_t index = 0)
  {
    if(size() <= index)
    {
      return dummy();
    }
    switch(priority)
    {
      case NotePriority::LAST: return played_note(size() - 1 - index);
      case NotePriority::LOW: return sorted_note(index);
      case NotePriority::HIGH: return sorted_note(size() - 1 - index);
      case NotePriority::FIRST: return played_note(index);

      default: return dummy();
    }
  }

private:
  uint8_t   size_;
  NoteEntry pool_[capacity + 1];       // First element is a dummy node!
  uint8_t   root_idx_;                 // Base 1.
  uint8_t   sorted_idx_[capacity + 1]; // Base 1.

  // DISALLOW_COPY_AND_ASSIGN(NoteStack);
};

} // namespace jamstack

#endif // JAMSTACK_NOTE_STACK_H_
