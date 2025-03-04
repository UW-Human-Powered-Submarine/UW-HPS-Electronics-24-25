#include "PressureSensor.h"

PressureSensor::PressureSensor()
    : PressureSensor(100000) {
}

PressureSensor::PressureSensor(unsigned long refresh_period_ms) 
    : Scheduler(refresh_period_ms)
    , sensor(), initialized(false), is_depth_calibrated(false), depth_zero_calibration(0.0f)
    , pressure_mbar(0.0f), depth_m(0.0f), temperature_C(0.0f) {
}

PressureSensor::~PressureSensor() {
}

void PressureSensor::begin() {
    Wire.begin();

    this->sensor.setModel(MS5837::MS5837_02BA);
    this->sensor.init();
    
    this->sensor.setFluidDensity(DENSITY_FRESH_WATER_KGPM);

    this->initialized = true;
}

float PressureSensor::get_pressure_mbar() const {
    return pressure_mbar;
}

float PressureSensor::get_depth_m() const {
    return depth_m;
}

float PressureSensor::get_temperature_c() const {
    return temperature_C;
}


void PressureSensor::calibrate_depth_zero(float zero_depth_m) {
    this->depth_zero_calibration = zero_depth_m;
    this->is_depth_calibrated = true;
}

void PressureSensor::calibrate_depth_zero_using_current_reading() {
    if (!this->initialized) {
        Serial.println("Pressure Sensor is not initialized. Use PresureSensor.begin() first.");
        return;
    }
    this->sensor.read();
    this->depth_zero_calibration = sensor.depth();

    this->is_depth_calibrated = true;
}

void PressureSensor::in_fresh_water() {
    this->sensor.setFluidDensity(DENSITY_FRESH_WATER_KGPM);
}

void PressureSensor::in_sea_water() {
    this->sensor.setFluidDensity(DENSITY_SEA_WATER_KGPM);
}

void PressureSensor::fluid_density(float density_kgpm3) {
    this->sensor.setFluidDensity(density_kgpm3);
}

void PressureSensor::event() {
    this->pull_sensor_data();
}

void PressureSensor::pull_sensor_data() {
    if (!this->initialized) {
        Serial.println("Pressure Sensor is not initialized. Use PresureSensor.begin() first.");
        return;
    }
    if (!this->is_depth_calibrated) {
        Serial.println("Depth is not calibrated. Use PressureSensor.calibrate_depth_zero() first.");
        return;
    }

    this->sensor.read();

    this->pressure_mbar = this->sensor.pressure(); 
    this->temperature_C = this->sensor.temperature(); 
    this->depth_m = this->sensor.depth() - this->depth_zero_calibration; 
}
