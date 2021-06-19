#ifndef UI_UART_QUEUE
#define UI_UART_QUEUE

#include <stdint.h>

namespace coreui
{
struct QueueEntry
{
  uint8_t item;
  uint8_t state;
  uint8_t next_idx; // Base 1.
};

static const uint8_t kFreeSlot = 0xFF;

template <uint8_t capacity>
class UartQueue
{
public:
  UartQueue() {}
  ~UartQueue() {}

  void Init() { Clear(); }
  void AddEntry(uint8_t item, uint8_t state)
  {
    // Remove the item from the list first (in case it is already here).
    RemoveEntry(item);
    // In case of saturation, remove the least recently played item from the
    // stack.

    if(size_ == capacity)
    {
      uint8_t least_recent_note = 1;
      for(uint8_t i = 1; i <= capacity; ++i)
      {
        if(pool_[i].next_idx == 0)
        {
          least_recent_note = pool_[i].item;
        }
      }
      RemoveEntry(least_recent_note);
    }

    // Now we are ready to insert the new item. Find a free slot to insert it.
    uint8_t free_slot = 1;
    for(uint8_t i = 1; i <= capacity; ++i)
    {
      if(pool_[i].item == kFreeSlot)
      {
        free_slot = i;
        break;
      }
    }
    pool_[free_slot].next_idx = root_idx_;
    pool_[free_slot].item     = item;
    pool_[free_slot].state    = state;
    root_idx_                 = free_slot;

    // The last step consists in inserting the item in the sorted list.
    for(uint8_t i = 0; i < size_; ++i)
    {
      if(pool_[sorted_idx_[i]].item > item)
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

  void RemoveEntry(uint8_t item)
  {
    uint8_t current  = root_idx_;
    uint8_t previous = 0;

    while(current)
    {
      if(pool_[current].item == item)
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
      pool_[current].item     = kFreeSlot;
      pool_[current].state    = 0;
      --size_;
    }
  }
  void Clear()
  {
    size_ = 0;
    memset(pool_ + 1, 0, sizeof(QueueEntry) * capacity);
    memset(sorted_idx_ + 1, 0, capacity);
    root_idx_ = 0;

    for(uint8_t i = 0; i <= capacity; ++i)
      pool_[i].item = kFreeSlot;
  }

  uint8_t           max_size() const { return capacity; }
  uint8_t           size() const { return size_; }
  const QueueEntry& most_recent_entry() const { return pool_[root_idx_]; }
  const QueueEntry& entry(uint8_t index) const { return pool_[index]; }

private:
  uint8_t    size_;
  QueueEntry pool_[capacity + 1];
  uint8_t    root_idx_;
  uint8_t    sorted_idx_[capacity + 1];
};

} // namespace coreui

#endif /* UI_UART_QUEUE */
