#include "IMU.h"

IMU::IMU() {
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
    // Cleanup code here
}

// Getter methods
int16_t IMU::get_x_acceleration() const {
    return x_acceleration;
}

int16_t IMU::get_y_acceleration() const {
    return y_acceleration;
}

int16_t IMU::get_z_acceleration() const {
    return z_acceleration;
}

int16_t IMU::get_x_gyro() const {
    return x_gyro;
}

int16_t IMU::get_y_gyro() const {
    return y_gyro;
}

int16_t IMU::get_z_gyro() const {
    return z_gyro;
}

int16_t IMU::get_temperature() const {
    return temperature;
}

