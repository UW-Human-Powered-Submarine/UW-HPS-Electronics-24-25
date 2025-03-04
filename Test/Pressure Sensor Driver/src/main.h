#ifndef ___MAIN_H___
#define ___MAIN_H___


#include <Arduino.h>

#define PRESSURE_LIB_TEST
// #define PRESSURE_SCHEDULER_TEST
// #define BLINKING_TEST


#if defined(PRESSURE_LIB_TEST)
//  +-------------------------+
//  |   PRESSURE_LIB_TEST     |
//  +-------------------------+

//  This is the sample script from BlueRobotics website:
//  https://bluerobotics.com/learn/guide-to-using-the-bar02-with-an-arduino/

#include <Wire.h>
#include "MS5837.h"

MS5837 sensor; 

//  endif IMU_LIB_TEST
#elif defined(PRESSURE_SCHEDULER_TEST)
//  +-------------------------+
//  | PRESSURE_SCHEDULER_TEST |
//  +-------------------------+
#include <Wire.h>
#include "MS5837.h"

//  endif IMU_PITCH_LIB_TEST
#elif defined(BLINKING_TEST)
//  +-------------------------+
//  |      BLINKING_TEST      |
//  +-------------------------+

#endif


#endif      //___MAIN_H___