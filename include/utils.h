/**
 * @file utils.h

 * @brief This header contains miscellaneous functions.
**/
#pragma once
#include <cstdarg>

template <unsigned QueueSize>
class Queue {
    const int kFlagsOverRun_ = 1;

    unsigned char buf_[QueueSize];
    int next_pos_write_ = 0, next_pos_read_ = 0;
    int num_free_ = 32;
    int flags_ = 0;

public:
    bool Enqueue(unsigned char data);
    int Dequeue();
    int GetNumElements();
};

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
    return data;
}

template <unsigned QueueSize>
int Queue<QueueSize>::GetNumElements()
{
    return QueueSize - num_free_;
}

int OSSPrintf(char* str, const char* format, ...);
int OSVSPrintf(char* str, const char* format, va_list ap);

template <typename T>
T Between(const T value, const T from, const T to)
{
    if (value < from) {
        return from;
    }

    if (value > to) {
        return to;
    }

    return value;
}
