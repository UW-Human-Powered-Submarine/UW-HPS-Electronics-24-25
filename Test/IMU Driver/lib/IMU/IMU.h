#ifndef ___IMU_PITCH_H___
#define ___IMU_PITCH_H___

#include "Wire.h"

#define MPU_ADDR 0x68

class IMU {
private:
    int16_t x_acceleration, y_acceleration, z_acceleration;  
    int16_t x_gyro, y_gyro, z_gyro;
    int16_t temperature;

public:
    IMU();
    ~IMU(); 

    //  Read x acceleration from last IMU reading
    int16_t get_x_acceleration() const;

    //  Read y acceleration from last IMU reading
    int16_t get_y_acceleration() const;

    //  Read z acceleration from last IMU reading
    int16_t get_z_acceleration() const;

    //  Read x gyro from last IMU reading
    int16_t get_x_gyro() const;

    //  Read y gyro from last IMU reading
    int16_t get_y_gyro() const;

    //  Read z gyro from last IMU reading
    int16_t get_z_gyro() const;

    //  Read temperature from last IMU reading
    int16_t get_temperature() const;
};

#endif  //  ___IMU_PITCH_H___