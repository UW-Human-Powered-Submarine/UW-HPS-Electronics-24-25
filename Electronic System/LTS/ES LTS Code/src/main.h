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

#include "FSMMacroV2.h"
#include "TimeStampMacro.h"

#include "IMU.h"
#include "PitchReading.h"
#include "PressureSensor.h"
#include "Charlieplex4Pin.h"

//  +------------------------------------- EEPROM --------------------------------------+

//  Version of the saved data saved in EEPROM
//  If EEPROM_VERSION matched to saved version, then the calibration data will be used,
//      otherwise, new calibration is required
//  Change this if saved data is incompatible with current program
#define EEPROM_VERSION 0


//  +------------------------------- Background Services -------------------------------+

IMU imu;
PitchReading pitch_reading;

PressureSensor pressure_sensor;

#define PINS_CHARLIEPLEX_HUD 2, 3, 4, 5
Charlieplex4PinBlink hud(PINS_CHARLIEPLEX_HUD);

//  +------------------------------------ Main Loop ------------------------------------+

enum MAIN_LOOP_FSM_STATES {
    ML_Initialization,
    ML_Idle_0, ML_GPVC_0, ML_GPVC_Start, ML_GPVC_Finished, ML_Idle_0_Finished,  //  GVPC: Gravity Vec Pressure Calibrate
    ML_Idle_1, ML_GPVC_1, ML_PVC_1, ML_PVC_1_Start, ML_PVC_1_Finished, ML_Idle_1_Finished,  //  PVC: Pitch Vec Calibrate
    ML_GPVC_2, ML_PVC_2, ML_SaveC, ML_SaveC_Finished,   //  SaveC: Save_Calibration
    ML_Active
};
CREATE_FSM(MAIN_LOOP, ML_Initialization);
void main_loop_update();


//  +------------------------- Blinking Builtin LED, Status LED ------------------------+
#define PIN_LED_BUILTIN 13
#define DELAY_INTERVAL_LED_BUILTIN 500
CREATE_FSM(BLINK, 0)
void blink_update();



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