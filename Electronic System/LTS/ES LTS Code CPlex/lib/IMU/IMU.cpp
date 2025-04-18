#include "IMU.h"

IMU::IMU(): IMU(IMU_ADDR_DEFAULT, 100000) {
}

IMU::IMU(uint8_t imu_address, unsigned long refresh_period_us)
    : Scheduler(refresh_period_us)
    , initialized(false), imu_address(imu_address)
    , x_acceleration(0), y_acceleration(0), z_acceleration(0)
    , x_gyro(0), y_gyro(0), z_gyro(0), temperature(0) {
}

IMU::~IMU() {
}

void IMU::begin() {
    // Begin the I2C
    Wire.begin();
    // begin communication with the IMU
    Wire.beginTransmission(this->imu_address);
    Wire.write(0x6B); // data from registor that we want, the gyroscope
    Wire.write(0); // wakes up the 6050
    // can begin or stop transmission from the arduino to IMU
    Wire.endTransmission(true);

    this->initialized = true;
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

Vector3D IMU::get_acceleration_vec() const {
    return Vector3D(this->x_acceleration, this->y_acceleration, this->z_acceleration);
}

Vector3D IMU::get_gyro_vec() const {
    return Vector3D(this->x_gyro, this->y_gyro, this->z_gyro);
}

void IMU::event() {
    if (!this->initialized) {
        Serial.println("IMU haven't initialized yet. Run IMU.begin() first. ");
        return;
    }

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
