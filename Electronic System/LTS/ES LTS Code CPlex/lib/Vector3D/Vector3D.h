#ifndef ___VECTOR3D_H___
#define ___VECTOR3D_H___

#include "Arduino.h"
#include "math.h"

//  +-------------------------------------------------------------------------+
//  |   Basic 3d vector operations.                                           | 
//  |                                                                         |
//  |   Just enough vector operations for pitch calculation                   |
//  |   -   dot product                                                       |
//  |   -   scalar multiplication                                             |
//  |   -   normalization                                                     |
//  |   -   vector addition                                                   |
//  |   -   norm (L2)                                                         |
//  |   -   projection                                                        |  
//  |   -   angle to another vector, in radian                                |                   
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    February 25, 2025                                     |
//  +-------------------------------------------------------------------------+

class Vector3D {
public:
    Vector3D();
    Vector3D(float x, float y, float z);
    Vector3D(const Vector3D& other);
    ~Vector3D();

    float get_x() const;
    float get_y() const;
    float get_z() const;

    float dot(const Vector3D& other) const;
    Vector3D scalar_multiply(const float scalar) const;
    Vector3D normalize() const;
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(float scalar) const;
    float norm() const;
    Vector3D project_to(const Vector3D& other) const;

    //  return radian
    float angle_to(const Vector3D& other) const;

    float angle_to_in_deg(const Vector3D& other) const;

    //  print to serial port
    void println() const;

    //  print to serial port
    static void print_vector(const Vector3D& vec);

private:
    float x, y, z;

};

#endif  // ___VECTOR3D_H___