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
#include "MS5837_FSM.h"
#include "DepthReading.h"
#include "SegDisplay.h"

#include "AverageSampling.h"

//  +-------------------------------------- PINS ---------------------------------------+

#define PIN_BTN_CBGV    2
#define PIN_BTN_CBPCH   3
#define PIN_BTN_SAVE    4

#define PIN_LED_CBGV    5
#define PIN_LED_CBPCH   6
#define PIN_LED_SAVE    7
#define PIN_LED_BUILTIN 13

#define PIN_7SEG_CLK    9
#define PIN_7SEG_DIO    8

#define PIN_BAT_VOLTAGE A0

//  +------------------------------------- EEPROM --------------------------------------+

//  Version of the saved data saved in EEPROM
//  If EEPROM_VERSION matched to saved version, then the calibration data will be used,
//      otherwise, new calibration is required
//  Change this if saved data is incompatible with current program
const unsigned char EEPROM_VERSION = 0;

#define ADDR_EEPROM_VERSION 43
#define ADDR_CALIBRATIONS (ADDR_EEPROM_VERSION + sizeof(EEPROM_VERSION))

//  The Structure of calibration data in EEPROM
struct SavedCalibration {
    float x_gravity; 
    float y_gravity; 
    float z_gravity; 

    float x_pitch; 
    float y_pitch; 
    float z_pitch; 

    float depth_zero;
};

//  Check if the eeprom version stored on device matched with specified version
bool is_eeprom_version_matched();

SavedCalibration retrieve_calibration_from_eeprom();
void save_calibration_to_eeprom(const SavedCalibration & calibrations);


//  +---------------------------------- Input Buttons ----------------------------------+

//  Reading button state. Buttons are active low, but macro have inverted the states
//      true when button is pressed. 
#define BTN_CBGV (!digitalRead(PIN_BTN_CBGV))
#define BTN_CBPCH (!digitalRead(PIN_BTN_CBPCH))
#define BTN_SAVE (!digitalRead(PIN_BTN_SAVE))

//  +------------------------------- Background Services -------------------------------+

IMU imu;
PitchReading pitch_reading;

MS5837_FSM ms5873;
DepthReading depth_reading;

SegDisplay segmented_display(PIN_7SEG_CLK, PIN_7SEG_DIO);

//  +------------------------------------ Main Loop ------------------------------------+

enum MAIN_LOOP_FSM_STATES {
    ML_Initialization,
    ML_Idle_0, ML_GPVC_0, ML_GPVC_Start, ML_GPVC_Reading, ML_GPVC_Finished,  //  GVPC: Gravity Vec Pressure Calibrate
    ML_Idle_1, ML_GPVC_1, ML_PVC_1, ML_PVC_Start, ML_PVC_Reading, ML_PVC_Finished,  //  PVC: Pitch Vec Calibrate
    ML_GPVC_2, ML_PVC_2, ML_SaveC, ML_SaveC_Finished,   //  SaveC: Save_Calibration
    ML_Active
};
CREATE_FSM(MAIN_LOOP, ML_Initialization);
void main_loop_update();

TS_CREATE(MAIN_LOOP_TIMER, 0)

//  Calibration Sample Averages
AverageSampling<Vector3D> vec_cali;
AverageSampling<float> depth_cali;

//  +---------------------------------- HUD Interface ----------------------------------+

#define HUD_DISPLAY_REFRESH_PERIOD 200;

CREATE_FSM(HUD_INTERFACE_UPDATE, 0)
void hud_interface_update();

//  +------------------------- Blinking Builtin LED, Status LED ------------------------+

#define DELAY_IDLE_LED_BUILTIN 1000
#define DELAY_ACTIVE_LED_BUILTIN 250

CREATE_FSM(BLINK, 0)
void blink_update();

//  +------------------------------------- Logging -------------------------------------+

CREATE_FSM(LOGGING, 0)
void logging_update();

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