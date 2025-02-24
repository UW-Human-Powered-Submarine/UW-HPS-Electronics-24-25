#include "IMU.h"

IMU::IMU(): IMU(100){
}

IMU::IMU(unsigned int refresh_period_us)
    : x_acceleration(0), y_acceleration(0), z_acceleration(0)
    , x_gyro(0), y_gyro(0), z_gyro(0), temperature(0)
    , prev_timestamp_us(refresh_period_us) {
    
    // Begin the I2C
    Wire.begin();
    // begin communication with the IMU
    Wire.beginTransmission(MPU_ADDR_DEFAULT);
    Wire.write(0x6B); // data from registor that we want, the gyroscope
    Wire.write(0); // wakes up the 6050
    // can begin or stop transmission from the arduino to IMU
    Wire.endTransmission(true);
}

IMU::~IMU() {
}

void IMU::update() {
    //  This method is similar to other library method. 
    //  I choose not to make this into a parent method and override because
    //      I am trying to reduce the library dependency. 

    unsigned long curr_time = micros();
    if (
        (this->prev_timestamp_us + this->refresh_period_us <= curr_time) 
        || (curr_time < this->prev_timestamp_us)   //  detect micros() overflow
    ) {
        // Serial.println(curr_time);
        this->prev_timestamp_us = curr_time;    //  record the new timestamp

        this->refresh();
    }
}

void IMU::refresh() {
}

void IMU::set_refresh_period(unsigned long microsecond){
    this->prev_timestamp_us = microsecond;
}

void IMU::set_refresh_period_ms(unsigned long millisecond) {
    this->prev_timestamp_us = millisecond * 1000;
}

int16_t IMU::get_x_acceleration() const {
    return this->x_acceleration;
}

int16_t IMU::get_y_acceleration() const {
    return this->y_acceleration;
}

int16_t IMU::get_z_acceleration() const {
    return this->z_acceleration;
}

int16_t IMU::get_x_gyro() const {
    return this->x_gyro;
}

int16_t IMU::get_y_gyro() const {
    return this->y_gyro;
}

int16_t IMU::get_z_gyro() const {
    return this->z_gyro;
}

int16_t IMU::get_temperature() const {
    return this->temperature;
}

