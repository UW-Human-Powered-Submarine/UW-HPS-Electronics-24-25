#include "Vector3D.h"

Vector3D::Vector3D() : Vector3D(0.0f, 0.0f, 0.0f) {
}

Vector3D::Vector3D(float x, float y, float z)
    : x(x), y(y), z(z) {
}

float Vector3D::get_x() const { 
    return x; 
}

float Vector3D::get_y() const { 
    return y; 
}

float Vector3D::get_z() const { 
    return z; 
}

float Vector3D::dot(const Vector3D &other) const {
    return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3D Vector3D::scalar_multiply(const float scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::normalize() const {
    float len = this->norm();
    if (abs(len) > 1e-8) {
        return *this * (1/len); 
    } else {
        // If the vector has zero length, return a zero vector (avoid division by zero)
        return Vector3D();
    }
}

Vector3D Vector3D::operator+(const Vector3D &other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator*(float scalar) const {
    return this->scalar_multiply(scalar);
}

float Vector3D::norm() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::project_to(const Vector3D &other) const {
    //  project A to B = ((A dot B) / (B dot B)) * B
    //  A: this
    //  B: other
    float A_dot_B = this->dot(other);
    float B_dot_B = other.dot(other);

    if (abs(B_dot_B) > 1e-8) {
        return other * (A_dot_B / B_dot_B);
    } else {
        // If the vector has zero length, return a zero vector (avoid division by zero)
        return Vector3D();
    }
}

float Vector3D::angle_to(const Vector3D &other) const {
    float A_norm = this->norm();
    float B_norm = other.norm();
    if ((abs(A_norm) > 1e-8) && (abs(B_norm) > 1e-8)) {
        return acos(this->dot(other) / (A_norm * B_norm));
    } else {
        // If the vector has zero length, return a zero vector (avoid division by zero)
        return 0.0f;
    }
}
