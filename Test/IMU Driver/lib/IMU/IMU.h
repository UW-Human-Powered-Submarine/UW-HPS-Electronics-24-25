#ifndef ___IMU_H___
#define ___IMU_H___

#include "Arduino.h"
#include "Wire.h"

#define IMU_ADDR_DEFAULT 0x68

class IMU {
private:
    uint8_t imu_address;
    int16_t x_acceleration, y_acceleration, z_acceleration;  
    int16_t x_gyro, y_gyro, z_gyro;
    int16_t temperature;

    unsigned long refresh_period_us = 100000;   //  default to 100ms, or 100,000us
    unsigned long prev_timestamp_us = 0;

public:
    IMU();
    IMU(uint8_t imu_address, unsigned long refresh_period_ms);
    ~IMU(); 

    //  update the value at given interval, could be called at highest frequency. 
    void update();

    //  forced refresh, update the sensor reading. 
    void refresh();

    void set_refresh_period(unsigned long microsecond);
    void set_refresh_period_ms(unsigned long millisecond);

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