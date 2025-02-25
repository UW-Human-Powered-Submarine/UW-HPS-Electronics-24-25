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

class PitchReading {
private:
    //  pointer to the IMU where to get the accelerometer data
    IMU *imu = nullptr;

    float unit_vec_gravity_calibration[3];
    float unit_vec_pitch_direction_calibration[3];
public:
    PitchReading(); 
    ~PitchReading(); 
};

#endif  // ___PITCH_READING_H___