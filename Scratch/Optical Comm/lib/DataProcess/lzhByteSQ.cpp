#include "lzhByteSQ.h"

LZHZByteSQ::LZHZByteSQ(int max_size)
    : LZHStackQueue<unsigned char>(max_size) {
}

LZHZByteSQ::~LZHZByteSQ() {
}

unsigned char LZHZByteSQ::max_val() const {
    if (this->item_size == 0) return 0;

    unsigned char max_value = this->arr[this->head];    //  get_item(0)
    for (int i = 1; i < this->item_size; i++) {
        unsigned char item = this->get_item(i);
        if (max_value < item) {
            max_value = item;
        }
    }
    
    return max_value;
}

unsigned char LZHZByteSQ::min_val() const {
    if (this->item_size == 0) return 0;

    unsigned char mim_value = this->arr[this->head];    //  get_item(0)
    for (int i = 1; i < this->item_size; i++) {
        unsigned char item = this->get_item(i);
        if (mim_value > item) {
            mim_value = item;
        }
    }
    
    return mim_value;
}
