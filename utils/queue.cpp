#include "utils.h"

template <unsigned QueueSize>
bool Queue<QueueSize>::Enqueue(unsigned char data)
{
    if (num_free_ == 0) {
        flags_ |= kFlagsOverRun_;
        return false;
    }

    buf_[next_pos_write_] = data;
    next_pos_write_ = (next_pos_write_ + 1) % QueueSize;

    num_free_--;
    return true;
}

template <unsigned QueueSize>
int Queue<QueueSize>::Dequeue()
{
    if (num_free_ == QueueSize) {
        return false;
    }

    int data = buf_[next_pos_read_];
    next_pos_read_ = (next_pos_read_ + 1) % QueueSize;

    num_free_++;
    return next_pos_read_;
}

template <unsigned QueueSize>
int Queue<QueueSize>::CountElements()
{
    return QueueSize - num_free_;
}
