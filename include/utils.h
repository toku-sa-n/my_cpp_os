template <unsigned QueueSize>
class Queue {
    // TODO: Make the size of buf_ changeable.
    const int kFlagsOverRun_ = 1;

    unsigned char buf_[QueueSize];
    int next_pos_write_ = 0, next_pos_read_ = 0;
    int num_free_ = 32;
    int flags_ = 0;

    bool Enqueue(unsigned char data);
    int Dequeue();
    int CountElements();
};

int OSSPrintf(char* str, const char* format, ...);
