#include "PitchReading.h"

PitchReading::PitchReading() : PitchReading(100000) {
}

PitchReading::PitchReading(unsigned long refresh_period_us) 
    : Scheduler(refresh_period_us) {
}

PitchReading::~PitchReading() {
}

float PitchReading::get_pitch_deg() const {
    if (!is_vec_gravity_calibrated || !is_vec_pitch_direction_calibrated) {
        Serial.println("PitchReading haven't calibrated yet. ");
    }
    return this->pitch_deg;
}

bool PitchReading::get_is_vec_gravity_calibrated() const {
    return this->is_vec_gravity_calibrated;
}

bool PitchReading::get_is_vec_pitch_direction_calibrated() const {
    return this->is_vec_pitch_direction_calibrated;
}

void PitchReading::event() {
    if (!is_vec_gravity_calibrated || !is_vec_pitch_direction_calibrated) {
        Serial.println("PitchReading haven't calibrated yet. ");
        return;
    }


}
