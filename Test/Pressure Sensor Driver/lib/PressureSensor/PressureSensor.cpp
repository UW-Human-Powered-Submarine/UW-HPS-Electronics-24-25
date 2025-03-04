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
    
    this->sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
}

void PressureSensor::event() {
}

void PressureSensor::pull_sensor_data() {
    if (!this->initialized || !this->is_depth_calibrated) {
        return;
    }

    this->sensor.read();

    this->pressure_mbar = this->sensor.pressure(); 
    this->temperature_C = this->sensor.temperature(); 
    this->depth_m = this->sensor.depth(); 
}
