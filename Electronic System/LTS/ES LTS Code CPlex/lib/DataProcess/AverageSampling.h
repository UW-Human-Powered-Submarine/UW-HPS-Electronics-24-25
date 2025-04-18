#ifndef __AVERAGE_SAMPLING_H__
#define __AVERAGE_SAMPLING_H__

#include "Vector3D.h"

template <typename T>
class AverageSampling {
public:
    AverageSampling(); 
    ~AverageSampling();

    int get_sample_size() const;
    T get_sample_average() const;

    void add_sample(const T &sample);
    void clear();
private:
    T sample_average; 
    int sample_size;
};

#include "AverageSampling.inl"
#endif // __AVERAGE_SAMPLING_H__
