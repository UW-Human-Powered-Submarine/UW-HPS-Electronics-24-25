#ifndef ___MAIN_H___
#define ___MAIN_H___

#include "Arduino.h"

// #define BLINKING_TEST

# include "Wire.h"

#define REFRESH_PERIOD 500

const int MPU_ADDR= 0x68;

// variables for acceleration in x,y and z, only these variables are ones to consider
int16_t x_acceleration, y_acceleration, z_acceleration;  
// variables for the gyroscope
int16_t x_gyro, y_gyro, z_gyro;
// variable for temperature
int16_t temperature;


char tmp_str[7];
char* convert_int16_to_str(int16_t i);


#endif  //  ___MAIN_H___