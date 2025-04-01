#ifndef ___LZH_BYTE_SQ_H___
#define ___LZH_BYTE_SQ_H___

#include "lzhStackQueue.h"

class LZHZByteSQ: public LZHStackQueue<unsigned char> {
public:
    LZHZByteSQ() = delete;
    LZHZByteSQ(int max_size);
    ~LZHZByteSQ();
    unsigned char max_val() const;
    unsigned char min_val() const;
private:
};


#endif  //  ___LZH_BYTE_SQ_H___