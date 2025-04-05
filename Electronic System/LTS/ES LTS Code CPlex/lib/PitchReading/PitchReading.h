#ifndef ___PITCH_READING_H___
#define ___PITCH_READING_H___

//  +-------------------------------------------------------------------------+
//  |   This will calculate the pitch periodically given of IMU sensor.       |
//  |   If calculating at fixed interval is not suitable,                     |
//  |       use PitchReading.refresh_and_get_pitch_deg()                              |
//  |                                                                         |
//  |   Must caliabrate gravity vector and pitch direction vector before use  |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    February 25, 2025                                     |
//  +-------------------------------------------------------------------------+

#include "IMU.h"
#include "Scheduler.h"
#include "Vector3D.h"

class PitchReading: public Scheduler {
public:
    PitchReading();
    PitchReading(unsigned long refresh_period_us); 

    ~PitchReading(); 

    void calibrate_gravity(const Vector3D &vec);
    void calibrate_pitch_direction(const Vector3D &vec);

    void register_imu(IMU *imu);

    float get_pitch_deg() const;
    float refresh_and_get_pitch_deg();

    bool get_is_vec_gravity_calibrated() const;
    bool get_is_vec_pitch_direction_calibrated() const;

    Vector3D get_gravity_calibration() const;
    Vector3D get_pitch_direction_calibration() const;
private:
    //  pointer to the IMU where to get the accelerometer data
    IMU *imu;

    bool is_vec_gravity_calibrated;
    Vector3D unit_vec_gravity_calibration;

    bool is_vec_pitch_direction_calibrated;
    Vector3D unit_vec_pitch_direction_calibration;

    float pitch_deg;

    void event() override;

    void calculate_angle_deg();
};

#endif  // ___PITCH_READING_H___