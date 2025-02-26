#ifndef ___IMU_H___
#define ___IMU_H___

//  +-------------------------------------------------------------------------+
//  |   This IMU class handles all the basic communication with MPU-6050 via  |
//  |       I2C protocal, and refresh automatically at set interval           |
//  |                                                                         |
//  |   IMU must be initialized before use by IMU.begin()                     |
//  |   IMU values are buffered in the internal field, calling IMU.refresh()  |
//  |       to pull data from IMU and update the field                        |
//  |   Calling IMU.update() at high frequency will automatically update      |
//  |       internal field at set interval.                                   |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    February 25, 2025                                     |
//  |                                                                         |
//  |   Special Thanks to Leo Lin for implementing the low-level              |
//  |       communication with MPU-6050                                       |
//  +-------------------------------------------------------------------------+

#include "Arduino.h"
#include "Wire.h"
#include "Scheduler.h"
#include "Vector3D.h"

#define IMU_ADDR_DEFAULT 0x68

class IMU: public Scheduler {
public:
    IMU();
    IMU(uint8_t imu_address, unsigned long refresh_period_ms);
    ~IMU(); 

    //  initialize IMU, start the communication
    void begin();

    int16_t get_x_acceleration() const;
    int16_t get_y_acceleration() const;
    int16_t get_z_acceleration() const;

    int16_t get_x_gyro() const;
    int16_t get_y_gyro() const;
    int16_t get_z_gyro() const;

    int16_t get_temperature() const;

    Vector3D get_acceleration_vec() const;
    Vector3D get_gyro_vec() const;

private:
    bool initialized;
    uint8_t imu_address;
    int16_t x_acceleration, y_acceleration, z_acceleration;  
    int16_t x_gyro, y_gyro, z_gyro;
    int16_t temperature;

    void event() override;
};

#endif  //  ___IMU_PITCH_H___