#include "lzhStackQueue.h"

template <typename T>
LZHStackQueue<T>::LZHStackQueue(int max_size)
    :arr(new T[max_size]), item_max_size(max_size), item_size(0), head(0), tail(0) {
}

template <typename T>
LZHStackQueue<T>::~LZHStackQueue() {
    delete[] arr;
}

template <typename T>
void LZHStackQueue<T>::push_front(T item) {
    if (this->is_full()) return;

    this->head = (this->head + 1) % this->item_max_size;
    this->arr[this->head] = item;
    item_size++;
}

template <typename T>
void LZHStackQueue<T>::push_back(T item) {
    if (this->is_full()) return;

    this->arr[this->tail] = item;
    this->tail = (this->tail + this->item_max_size - 1) % this->item_max_size;
    item_size++;
}

template <typename T>
T LZHStackQueue<T>::pop_front() {
    if (this->is_empty()) return T();
    
    int prev_head = this->head;
    this->head = (this->head + this->item_max_size - 1) % this->item_max_size;
    item_size--;
    return this->arr[prev_head];
}

template <typename T>
T LZHStackQueue<T>::pop_back() {
    if (this->is_empty()) return T();

    this->tail = (this->tail + 1) % this->item_max_size;
    item_size--;
    return this->arr[this->tail];
}

template <typename T>
inline T LZHStackQueue<T>::peek_front() const {
    if (this->is_empty()) return T();

    return this->arr[this->head];
}

template <typename T>
inline T LZHStackQueue<T>::peek_back() const {
    if (this->is_empty()) return T();

    int back_idx = (this->tail + 1) % this->item_max_size;
    return this->arr[back_idx];
}

template <typename T>
inline int LZHStackQueue<T>::size() const {
    return this->item_size;
}

template <typename T>
inline int LZHStackQueue<T>::max_size() const {
    return this->item_max_size;
}

template <typename T>
inline bool LZHStackQueue<T>::is_empty() const {
    return this->size() == 0;
}

template <typename T>
inline bool LZHStackQueue<T>::is_full() const {
    return this->item_size == item_max_size;
}

template <typename T>
T LZHStackQueue<T>::get_item(int index) const {
    if (index >= this->item_size || index < 0) return T();

    return this->arr[(this->head + this->item_max_size - index) % this->item_max_size];
}
