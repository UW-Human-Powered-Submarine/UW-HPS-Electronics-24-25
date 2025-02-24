#include "IMU.h"

IMU::IMU(): IMU(IMU_ADDR_DEFAULT, 100000) {
}

IMU::IMU(uint8_t imu_address, unsigned long refresh_period_us)
    : imu_address(imu_address), x_acceleration(0), y_acceleration(0), z_acceleration(0)
    , x_gyro(0), y_gyro(0), z_gyro(0), temperature(0)
    , refresh_period_us(refresh_period_us), prev_timestamp_us(0) {
    
    // Begin the I2C
    Wire.begin();
    // begin communication with the IMU
    Wire.beginTransmission(this->imu_address);
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
    Wire.beginTransmission(this->imu_address);
    Wire.write(0x3B); // gets data from registor we want
    Wire.endTransmission(false); // restart arudnio, but keep up communications

    //  There is some ambiguity when casting from bool -> uint8_t, 
    //      but no issue from bool -> int -> uint8_t, so, I cast the imu_address 
    //      to int to make the method signiture appear as Wire.requestFrom(int, int, int)
    Wire.requestFrom((int)(this->imu_address), 7*2, true); // requesting 14 registors 

    // reads the  acceleration
    this->x_acceleration = Wire.read() << 8 | Wire.read(); 
    this->y_acceleration = Wire.read() << 8 | Wire.read();
    this->z_acceleration = Wire.read() << 8 | Wire.read();

    // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    this->temperature = Wire.read()<<8 | Wire.read(); 

    // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    this->x_gyro = Wire.read()<<8 | Wire.read(); 
    // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    this->y_gyro = Wire.read()<<8 | Wire.read(); 
    // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
    this->z_gyro = Wire.read()<<8 | Wire.read(); 
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

