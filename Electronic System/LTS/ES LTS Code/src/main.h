#ifndef ___MAIN_H___
#define ___MAIN_H___

//  Comment and Uncomment following lines to choose program fucntion
#define ELECTRONIC_SYSTEM
// #define SYSTEM_TEST


#if defined(ELECTRONIC_SYSTEM)
//  +---------------------------------------------------------------------------------------------+
//  |                                      Electronic System                                      |
//  +---------------------------------------------------------------------------------------------+
#include "Arduino.h"
#include "EEPROM.h"
#include "Wire.h"

#include "FiniteStateMachineMacro.h"


//  +-----------------------------------------------------------------------------------+
//  |                          Blinking Builtin LED, Status LED                         |
//  +-----------------------------------------------------------------------------------+
#define PIN_LED_BUILTIN 13
#define DELAY_INTERVAL_LED_BUILTIN 500
FSM_CREATE_W_SCHEDULER(BLINK, 0)



#elif defined(SYSTEM_TEST)
//  +---------------------------------------------------------------------------------------------+
//  |                                         System Test                                         |
//  +---------------------------------------------------------------------------------------------+
#include <Arduino.h>

#define PIN_BTN_CBGVT 8
#define PIN_BTN_CBPCH 9
#define PIN_BTN_SAVE  10

#endif 

#endif  //  ___MAIN_H___