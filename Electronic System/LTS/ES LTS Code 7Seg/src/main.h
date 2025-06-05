#ifndef ___MAIN_H___
#define ___MAIN_H___

//  UW-HPS Electronics System LTS Ver. 2025

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
#include "LEDControl.h"

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

#define PIN_EXT_PORT    A0
#define PIN_BAT_VOLTAGE A3
#define PIN_CHARGING    A2      //  Charging LED
#define PIN_STANDBY     A1      //  Standby LED, charging finished

//  +------------------------------------- EEPROM --------------------------------------+

//  Version of the saved data saved in EEPROM
//  If EEPROM_VERSION matched to saved version, then the calibration data will be used,
//      otherwise, new calibration is required
//  Change this if saved data is incompatible with current program
const unsigned char EEPROM_VERSION = 42;

#define ADDR_EEPROM_VERSION 0
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
void erase_eeprom();


//  +---------------------------------- Input Buttons ----------------------------------+

//  Reading button state. Buttons are active low, but macro have inverted the states
//      true when button is pressed. 
#define BTN_CBGV (!digitalRead(PIN_BTN_CBGV))
#define BTN_CBPCH (!digitalRead(PIN_BTN_CBPCH))
#define BTN_SAVE (!digitalRead(PIN_BTN_SAVE))

//  +---------------------------------- Power Sensing ----------------------------------+

//  active low

#define PWR_CHARGING_THRESHOLD_B    40
#define PWR_STANDBY_THRESHOLD_B     40

#define PWR_UPDATE_PERIOD 200

float pwr_batt_volt_reading_raw;
float pwr_charging_reading_raw;
float pwr_standby_reading_raw;

//  Smoothing factor, lower is smoothier (weight on the current reading)

#define PWR_BATT_VOLT_SMOOTHING_FACTOR  0.1
#define PWR_CHARGING_SMOOTHING_FACTOR   0.1
#define PWR_STANDBY_SMOOTHING_FACTOR    0.1

#define PWR_GET_BATTERY_VOLTAGE     (pwr_batt_volt_reading_raw * 5.0 / 1024)

//  CRITERIA

//  Define as battery less than 3.2V
#define PWR_IS_BATT_LOW (PWR_GET_BATTERY_VOLTAGE < 3.2)

//  Define as battery less than 2.5V
#define PWR_IS_BATT_DISCONNECTED (PWR_GET_BATTERY_VOLTAGE < 2.5)

#define PWR_IS_CHARGING (pwr_charging_reading_raw < PWR_CHARGING_THRESHOLD_B)
#define PWR_IS_STANDBY  (pwr_standby_reading_raw < PWR_STANDBY_THRESHOLD_B)

//  power state 1
#define PWR_EVENT_OPEN_SWITCH (                                             \
    (PWR_IS_BATT_DISCONNECTED && PWR_IS_CHARGING)                           \
    || (PWR_IS_BATT_DISCONNECTED && !PWR_IS_CHARGING && PWR_IS_STANDBY))    \

//  power state 2
#define PWR_EVENT_BATT_FAULT (                                              \
    PWR_IS_BATT_DISCONNECTED && !PWR_IS_CHARGING && !PWR_IS_STANDBY)

//  power state 3
#define PWR_EVENT_CHARGING_STATE (                                          \
    !PWR_IS_BATT_DISCONNECTED && PWR_IS_CHARGING)

//  power state 4
#define PWR_EVENT_FINISHED_STATE (                                          \
    !PWR_IS_BATT_DISCONNECTED && !PWR_IS_CHARGING && PWR_IS_STANDBY)

//  power state 5
#define PWR_EVENT_BATT_LOW_STATE    PWR_IS_BATT_LOW

//  power state 0, put in the else statement
#define PWR_EVENT_NORMAL_OPT (                                              \
    !PWR_IS_BATT_DISCONNECTED && !PWR_IS_CHARGING && !PWR_IS_STANDBY)

//  Previous power state, used for resetting display scroling
int pwr_prev_state;

CREATE_FSM(POWER_SENSING_READING, 0)
void power_sensing_reading_update();

//  Require the decimal place 0b00100000
void fetch_battery_voltage_to_string(char *str_out);

//  This is an estimation to battery capacity. 
//  It is a linear interpolation between
void fetch_battery_percentage_to_string(char *str_out);

//  battery percentage conversion
#define PWR_V_BATT_MIN   3.1f
#define PWR_V_BATT_MAX   3.65f
#define PWR_PERCENT_BATT_MIN  0.0f
#define PWR_PERCENT_BATT_MAX  1.0f

#define PWR_BATTERY_PERCENTAGE (((PWR_GET_BATTERY_VOLTAGE) - PWR_V_BATT_MIN) / (PWR_V_BATT_MAX - PWR_V_BATT_MIN) * (PWR_PERCENT_BATT_MAX - PWR_PERCENT_BATT_MIN) + PWR_PERCENT_BATT_MIN)


//  +------------------------------- Background Services -------------------------------+

#define PITCH_READING_SMOOTHING_FACTOR 0.3
IMU imu;
PitchReading pitch_reading;

MS5837_FSM ms5873;
DepthReading depth_reading;

SegDisplay segmented_display(PIN_7SEG_CLK, PIN_7SEG_DIO);

LEDControl led_builtin(PIN_LED_BUILTIN);
LEDControl led_gravity_calibration(PIN_LED_CBGV);
LEDControl led_pitch_calibration(PIN_LED_CBPCH);
LEDControl led_epprom_save(PIN_LED_SAVE);

//  +------------------------------------ Main Loop ------------------------------------+

enum MAIN_LOOP_FSM_STATES {
    ML_Initialization,
    ML_Idle_0, ML_GPVC_0, ML_GPVC_Start, ML_GPVC_Reading, ML_GPVC_Finished,  //  GVPC: Gravity Vec Pressure Calibrate
    ML_Idle_1, ML_GPVC_1, ML_PVC_1, ML_PVC_Start, ML_PVC_Reading, ML_PVC_Finished,  //  PVC: Pitch Vec Calibrate
    ML_GPVC_2, ML_PVC_2, ML_SaveC, ML_SaveC_Finished, ML_EraseC, ML_EraseC_Finished,   //  SaveC: Save_Calibration
    ML_Action_Menu, ML_Disp_Batt_Menu, ML_Disp_Batt, ML_Disp_Batt2,  
    ML_Active                                                                          //  EraseC: Erase Calibration
};
CREATE_FSM(MAIN_LOOP, ML_Initialization);
void main_loop_update();

TS_CREATE(MAIN_LOOP_TIMER, 0)

//  Calibration Sample Averages
AverageSampling<Vector3D> vec_cali;
AverageSampling<float> depth_cali;

//  +---------------------------------- HUD Interface ----------------------------------+

#define HUD_DISPLAY_REFRESH_PERIOD 100
#define HUD_DISPLAY_TEXT_DELAY 1000

#define HUD_DISPLAY_TEXT_DELAY_MULTIPLIER (HUD_DISPLAY_TEXT_DELAY / HUD_DISPLAY_REFRESH_PERIOD)

const char HUD_EMPTY_MSG[7] = "      ";

struct SegDisplayConfigItem {
    float lower_range; 
    float upper_range;

    SegDisplayBlinkState status; 
};

#define DEPTH_DISPLAY_CONFIG_COUNT 3
const SegDisplayConfigItem DEPTH_DISPLAY_CONFIG[DEPTH_DISPLAY_CONFIG_COUNT] = {
    {-INFINITY, 1.,       SDBS_BLINK_FAST},
    {1.,        4.,       SDBS_ON},
    {4.,        INFINITY, SDBS_BLINK_FAST}
};

#define PITCH_DISPLAY_CONFIG_COUNT 3
const SegDisplayConfigItem PITCH_DISPLAY_CONFIG[PITCH_DISPLAY_CONFIG_COUNT] = {
    {-INFINITY, -45.,     SDBS_BLINK_FAST},
    {-45.,      45.,      SDBS_ON},
    {45.,       INFINITY, SDBS_BLINK_FAST}
};

//  show the pitch and depth reading in the provided configuration
void render_sensor_info();

enum HUD_INTERFACE_UPDATE_FSM_STATES {
    HIU_Welcome,
    HIU_HUB, HIU_HUB_Reset, HIU_Sensor_Info, 
    HIU_Idle_0, HIU_GPVC, 
    HIU_Idle_1, HIU_PVC, 
    HIU_Reading, HIU_Finish, HIU_Save, HIU_Erase, 
    HIU_Action_Menu, HIU_Disp_Batt_Menu, HIU_Disp_batt, HIU_Disp_batt2
};

unsigned int text_scroll_counter;

CREATE_FSM(HUD_INTERFACE_UPDATE, HIU_Welcome);
void hud_interface_update();

//  +------------------------------------ Status LED -----------------------------------+

#define DELAY_STATUS_LED_FAST_BLINK 200
#define DELAY_STATUS_LED_SLOW_BLINK 1000

CREATE_FSM(STATUS_LED, 0);
void status_led_update();

//  +-------------------------------- Blinking Builtin LED -----------------------------+

#define DELAY_IDLE_LED_BUILTIN 1000
#define DELAY_ACTIVE_LED_BUILTIN 500

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