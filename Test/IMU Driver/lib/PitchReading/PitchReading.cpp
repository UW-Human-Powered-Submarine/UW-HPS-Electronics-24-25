#include "PitchReading.h"

PitchReading::PitchReading() : PitchReading(100000) {
}

PitchReading::PitchReading(unsigned long refresh_period_us) : Scheduler(refresh_period_us)
    {
}

PitchReading::~PitchReading() {
}
