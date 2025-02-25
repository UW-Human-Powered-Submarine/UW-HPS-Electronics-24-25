#ifndef ___MAIN_H___
#define ___MAIN_H___

#include "Arduino.h"

#define IMU_LIB_TEST
// #define IMU_PITCH_LIB_TEST
// #define BLINKING_TEST


#if defined(IMU_LIB_TEST)
//  +-------------------------+
//  |      IMU_LIB_TEST       |
//  +-------------------------+
#include "IMU.h"

IMU imu;

//  endif IMU_LIB_TEST
#elif defined(IMU_PITCH_LIB_TEST)
//  +-------------------------+
//  |   IMU_PITCH_LIB_TEST    |
//  +-------------------------+
#include "IMU.h"

//  endif IMU_PITCH_LIB_TEST
#elif defined(BLINKING_TEST)
//  +-------------------------+
//  |      BLINKING_TEST      |
//  +-------------------------+

#endif


#endif  //  ___MAIN_H___