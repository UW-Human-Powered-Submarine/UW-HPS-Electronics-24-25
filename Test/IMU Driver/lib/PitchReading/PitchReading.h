#ifndef ___PITCH_READING_H___
#define ___PITCH_READING_H___

//  +-------------------------------------------------------------------------+
//  |   This will calculate the pitch periodically given of IMU sensor.       |
//  |   If calculating at fixed interval is not suitable,                     |
//  |       use PitchReading.refresh_pitch_deg()                              |
//  |                                                                         |
//  |   Must caliabrate gravity vector and pitch direction vector before use  |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    February 25, 2025                                     |
//  +-------------------------------------------------------------------------+

#include "IMU.h"
#include "Scheduler.h"


class PitchReading: public Scheduler {
public:
    PitchReading();
    PitchReading(unsigned long refresh_period_us); 

    ~PitchReading(); 

    float get_pitch_deg() const;
    bool get_is_vec_gravity_calibrated() const;
    bool get_is_vec_pitch_direction_calibrated() const;
private:
    //  pointer to the IMU where to get the accelerometer data
    IMU *imu = nullptr;

    bool is_vec_gravity_calibrated = false;
    float unit_vec_gravity_calibration[3];

    bool is_vec_pitch_direction_calibrated = false;
    float unit_vec_pitch_direction_calibration[3];

    float pitch_deg = 0.0f;

    void event() override;
};

#endif  // ___PITCH_READING_H___