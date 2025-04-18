#include "DepthReading.h"

DepthReading::DepthReading()
    : DepthReading(100000) {
}

DepthReading::DepthReading(unsigned long refresh_period_ms) 
    : Scheduler(refresh_period_ms)
    , sensor(nullptr), is_depth_calibrated(false), depth_zero_calibration(0.0f)
    , pressure_mbar(0.0f), depth_m(0.0f), temperature_C(0.0f) {
}

DepthReading::~DepthReading() {
}

void DepthReading::register_ms5873(MS5837_FSM *ms5873) {
    this->sensor = ms5873;
}

float DepthReading::get_pressure_mbar() const {
    return pressure_mbar;
}

float DepthReading::get_depth_m() const {
    return depth_m;
}

float DepthReading::get_temperature_c() const {
    return temperature_C;
}

float DepthReading::get_depth_calibration() const {
    return this->depth_zero_calibration;
}

void DepthReading::calibrate_depth_zero(float zero_depth_m) {
    this->depth_zero_calibration = zero_depth_m;
    this->is_depth_calibrated = true;
}

void DepthReading::calibrate_depth_zero_using_current_reading() {
    if (this->sensor == nullptr) {
        Serial.println("Pressure Sensor is not registered. Use PresureSensor.register_imu() first.");
        return;
    }

    this->depth_zero_calibration = this->sensor->depth();

    this->is_depth_calibrated = true;
}

void DepthReading::in_fresh_water() {
    this->sensor->setFluidDensity(DENSITY_FRESH_WATER_KGPM);
}

void DepthReading::in_sea_water() {
    this->sensor->setFluidDensity(DENSITY_SEA_WATER_KGPM);
}

void DepthReading::fluid_density(float density_kgpm3) {
    this->sensor->setFluidDensity(density_kgpm3);
}

void DepthReading::event() {
    this->pull_sensor_data();
}

void DepthReading::pull_sensor_data() {
    if (this->sensor == nullptr) {
        Serial.println("Pressure Sensor is not registered. Use PresureSensor.register_imu() first.");
        return;
    }
    if (!this->is_depth_calibrated) {
        Serial.println("Depth is not calibrated. Use DepthReading.calibrate_depth_zero() first.");
        return;
    }

    // this->sensor->read();

    this->pressure_mbar = this->sensor->pressure(); 
    this->temperature_C = this->sensor->temperature(); 
    this->depth_m = this->sensor->depth() - this->depth_zero_calibration; 
}
