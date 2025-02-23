#include "IMU.h"

IMU::IMU(): IMU(100){
}

IMU::IMU(unsigned int refresh_period_ms)
    : x_acceleration(0), y_acceleration(0), z_acceleration(0)
    , x_gyro(0), y_gyro(0), z_gyro(0), temperature(0), refresh_period_ms(refresh_period_ms) {
    
    // Begin the I2C
    Wire.begin();
    // begin communication with the IMU
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B); // data from registor that we want, the gyroscope
    Wire.write(0); // wakes up the 6050
    // can begin or stop transmission from the arduino to IMU
    Wire.endTransmission(true);
}

IMU::~IMU() {
}

void IMU::set_refresh_period(unsigned int millisecond){
    this->refresh_period_ms = millisecond;
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

