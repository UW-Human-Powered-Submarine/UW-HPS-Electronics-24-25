#include "AverageSampling.h"

template <typename T> 
AverageSampling<T>::AverageSampling(): sample_average(T()), sample_size(0) {
}

template <typename T>
AverageSampling<T>::~AverageSampling() {
}

template <typename T>
int AverageSampling<T>::get_sample_size() const {
    return sample_size;
}

template <typename T>
T AverageSampling<T>::get_sample_average() const {
    return sample_average;
}

template <typename T>
void AverageSampling<T>::add_sample(const T &sample) {
    this->sample_average =  this->sample_average * (((float) this->sample_size) / (this->sample_size + 1))
        + sample * (1.0f / (this->sample_size + 1));
    this->sample_size++;
}

template <typename T>
void AverageSampling<T>::clear() {
    this->sample_average = T();
    this->sample_size = 0;
}

// Explicit template instantiation
template class AverageSampling<float>;
template class AverageSampling<Vector3D>;
