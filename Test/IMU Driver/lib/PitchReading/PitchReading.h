#ifndef ___PITCH_READING_H___
#define ___PITCH_READING_H___

//  +-------------------------------------------------------------------------+
//  |   
//  |                                                                         |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    February 24, 2025                                     |
//  +-------------------------------------------------------------------------+

#include "IMU.h"
#include "Scheduler.h"

class PitchReading: public Scheduler {
public:
    PitchReading();
    PitchReading(unsigned long refresh_period_us); 

    ~PitchReading(); 
private:
    //  pointer to the IMU where to get the accelerometer data
    IMU *imu = nullptr;

    bool initialized = false;

    float unit_vec_gravity_calibration[3];
    float unit_vec_pitch_direction_calibration[3];
};

#endif  // ___PITCH_READING_H___