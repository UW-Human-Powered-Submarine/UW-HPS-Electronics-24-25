#ifndef ___LZH_STACK_QUEUE_H___
#define ___LZH_STACK_QUEUE_H___

template <typename T>
class LZHStackQueue {
public:
    LZHStackQueue(int max_size);
    ~LZHStackQueue();
    
    void push_front(T item);
    void push_back(T item);

    T pop_front();
    T pop_back();

    T peek_front() const;
    T peek_back() const;

    int size() const;
    int max_size() const;

    bool is_empty() const;
    bool is_full() const;

    T get_item(int index) const;
protected:
    T *const arr;
    const int item_max_size; 
    int item_size; 
    int head;   //  always point at the begin of the queue
    int tail;   //  always point at one behind the end of queue
};

#include "lzhStackQueue.inl"

#endif //   ___LZH_STACK_QUEUE_H___


