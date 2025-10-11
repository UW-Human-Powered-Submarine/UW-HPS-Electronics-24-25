#include "PitchReading.h"

PitchReading::PitchReading() : PitchReading(100000) {
}

PitchReading::PitchReading(unsigned long refresh_period_us) 
    : Scheduler(refresh_period_us) 
    , imu(nullptr)
    , is_vec_gravity_calibrated(false), unit_vec_gravity_calibration()
    , is_vec_pitch_direction_calibrated(false), unit_vec_pitch_direction_calibration()
    , pitch_deg(0.0f), pitch_deg_smoothing(0.0f), smoothing_factor(0.1) {
}

PitchReading::~PitchReading() {
}

void PitchReading::calibrate_gravity(const Vector3D &vec) {
    this->unit_vec_gravity_calibration = vec.normalize();
    this->is_vec_gravity_calibrated = true;
}

void PitchReading::calibrate_pitch_direction(const Vector3D &vec) {
    if (!is_vec_gravity_calibrated) {
        Serial.println("Calibrate Gravity vector first. ");
    }

    //  Gram-Schmit
    Vector3D pitch_direction = vec - vec.project_to(this->unit_vec_gravity_calibration);

    this->unit_vec_pitch_direction_calibration = pitch_direction.normalize();
    this->is_vec_pitch_direction_calibrated = true;
}

void PitchReading::register_imu(IMU *imu) {
    this->imu = imu;
}

float PitchReading::get_pitch_deg() const {
    if (!is_vec_gravity_calibrated || !is_vec_pitch_direction_calibrated) {
        Serial.println("PitchReading haven't calibrated yet. ");
    }
    return this->pitch_deg;
}

float PitchReading::get_pitch_deg_smoothed() const {
    return this->pitch_deg_smoothing;
}

float PitchReading::refresh_and_get_pitch_deg() {
    this->calculate_angle_deg();
    return this->pitch_deg;
}

void PitchReading::set_smoothing_factor(float smoothing_factor) {
    this->smoothing_factor = smoothing_factor;
}

bool PitchReading::get_is_vec_gravity_calibrated() const {
    return this->is_vec_gravity_calibrated;
}

bool PitchReading::get_is_vec_pitch_direction_calibrated() const {
    return this->is_vec_pitch_direction_calibrated;
}

Vector3D PitchReading::get_gravity_calibration() const {
    return this->unit_vec_gravity_calibration;
}

Vector3D PitchReading::get_pitch_direction_calibration() const {
    return this->unit_vec_pitch_direction_calibration;
}

void PitchReading::event() {
    if (!is_vec_gravity_calibrated || !is_vec_pitch_direction_calibrated) {
        Serial.println("PitchReading haven't calibrated yet. ");
        return;
    }

    if (imu == nullptr) {
        Serial.println("No regiestered IMU. Use register_imu() to add IMU");
        return; 
    }

    this->calculate_angle_deg();
}

void PitchReading::calculate_angle_deg() {
    if (!is_vec_gravity_calibrated || !is_vec_pitch_direction_calibrated) {
        Serial.println("PitchReading haven't calibrated yet. ");
        return;
    }

    Vector3D vec_acc = this->imu->get_acceleration_vec().normalize();

    if (vec_acc.norm() < 1e-8) {
        //  reading very close to zero. ignore this reading;
        return;
    }
    
    //  project into pitch plane
    Vector3D vec_acc_pitch_plane = 
        vec_acc.project_to(this->unit_vec_gravity_calibration) 
        + vec_acc.project_to(this->unit_vec_pitch_direction_calibration);

    float vec_dot_pitch_direction = vec_acc_pitch_plane.dot(this->unit_vec_pitch_direction_calibration);
    float sign = vec_dot_pitch_direction / abs(vec_dot_pitch_direction);

    this->pitch_deg = vec_acc_pitch_plane.angle_to_in_deg(this->unit_vec_gravity_calibration) * sign;
    this->pitch_deg_smoothing = this->smoothing_factor * pitch_deg
        + (1 - this->smoothing_factor) * pitch_deg_smoothing;
}
