#include "main.h"
#if defined(ELECTRONIC_SYSTEM)
//  +---------------------------------------------------------------------------------------------+
//  |                                      Electronic System                                      |
//  +---------------------------------------------------------------------------------------------+

void setup() {
    Serial.begin(9600);
    Wire.begin();

    //  initialize input buttons
    pinMode(PIN_BTN_CBGV,  INPUT_PULLUP);
    pinMode(PIN_BTN_CBPCH, INPUT_PULLUP);
    pinMode(PIN_BTN_SAVE,  INPUT_PULLUP);

    imu.set_refresh_period_ms(200);
    imu.begin();

    pitch_reading.set_refresh_period_ms(200);
    pitch_reading.register_imu(&imu);

    ms5873.set_refresh_period_ms(200);
    ms5873.setModel(MS5837::MS5837_02BA);
    ms5873.init();

    pressure_sensor.set_refresh_period_ms(200);
    pressure_sensor.register_ms5873(&ms5873);
    pressure_sensor.in_fresh_water();

    hud.set_fast_blink_period_ms(100);  //  5Hz
    hud.set_slow_blink_period_ms(500);  //  1Hz
}

void loop() {
    hud.update();

    if (GET_STATE(MAIN_LOOP) == ML_Active) {
        imu.update();
        pitch_reading.update();
        ms5873.update();
        pressure_sensor.update();
    } else if (
        GET_STATE(MAIN_LOOP) == ML_PVC_Reading 
        || GET_STATE(MAIN_LOOP) == ML_GPVC_Reading
    ) {
        imu.update();
        ms5873.update();
    }

    main_loop_update();
    main_display_interface_update();
    pitch_and_depth_display_update();

    blink_update();
    logging_update();
}
//  +------------------------------------- EEPROM --------------------------------------+

bool is_eeprom_version_matched() {
    unsigned char device_eeprom_version;
    EEPROM.get(ADDR_EEPROM_VERSION, device_eeprom_version);
    return device_eeprom_version == EEPROM_VERSION;
}

SavedCalibration retrieve_calibration_from_eeprom() {
    SavedCalibration ret_val;
    EEPROM.get(ADDR_CALIBRATIONS, ret_val);
    return ret_val;
}

void save_calibration_to_eeprom(const SavedCalibration & calibrations) {
    EEPROM.put(ADDR_CALIBRATIONS, calibrations);
}

//  +---------------------------------- Input Buttons ----------------------------------+

//  +------------------------------- Background Services -------------------------------+

//  +------------------------------------ Main Loop ------------------------------------+

void main_loop_update() {
    SETUP_FSM_FUNCTION(MAIN_LOOP)

    STATE(ML_Initialization) {
        if (is_eeprom_version_matched()) {
            //  retrieve calibration from eeprom
            SavedCalibration calibration = retrieve_calibration_from_eeprom();

            pitch_reading.calibrate_gravity(Vector3D(
                calibration.x_gravity, calibration.y_gravity, calibration.z_gravity
            ));
            pitch_reading.calibrate_pitch_direction(Vector3D(
                calibration.x_pitch, calibration.y_pitch, calibration.z_pitch
            ));
            pressure_sensor.calibrate_depth_zero(calibration.depth_zero);

            TO(ML_Active);
        }


        if (!is_eeprom_version_matched()) TO(ML_Idle_0);
    }

    STATE(ML_Idle_0) {
        if (BTN_CBGV) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_GPVC_0);
        }
    }

    STATE(ML_GPVC_0) {
        if (!BTN_CBGV) TO(ML_Idle_0);

        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) TO(ML_GPVC_Start);
    }

    STATE(ML_GPVC_Start) {
        TS_REFRESH(MAIN_LOOP_TIMER);

        vec_cali.clear();
        depth_cali.clear();
        // pressure_sensor.calibrate_depth_zero(0);    //  temporarily set pressure to absolute zero

        TO(ML_GPVC_Reading);
    }

    STATE(ML_GPVC_Reading) {
        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) {
            pitch_reading.calibrate_gravity(vec_cali.get_sample_average());
            pressure_sensor.calibrate_depth_zero(depth_cali.get_sample_average());
            TO(ML_GPVC_Finished);
        }

        //  refresh imu and pressure sensor
        // imu.refresh_no_timer_reset();
        // pressure_sensor.refresh_no_timer_reset();

        vec_cali.add_sample(imu.get_acceleration_vec());
        depth_cali.add_sample(ms5873.depth());
        // Serial.println(pressure_sensor.get_depth_m());

        SLEEP(200);
    }

    STATE(ML_GPVC_Finished) {
        if (!BTN_CBGV) TO(ML_Idle_1);
    }

    STATE(ML_Idle_1) {
        if (BTN_CBGV) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_GPVC_1);
        }
        if (BTN_CBPCH) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_PVC_1);
        }
    }

    STATE(ML_GPVC_1) {
        if (!BTN_CBGV) TO(ML_Idle_1);

        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) TO(ML_GPVC_Start);
    }

    STATE(ML_PVC_1) {
        if (!BTN_CBPCH) TO(ML_Idle_1);

        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) TO(ML_PVC_Start);
    }

    STATE(ML_PVC_Start) {
        TS_REFRESH(MAIN_LOOP_TIMER);
        vec_cali.clear();
        TO(ML_PVC_Reading);
    }

    STATE(ML_PVC_Reading) {
        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) {
            pitch_reading.calibrate_pitch_direction(vec_cali.get_sample_average());

            TO(ML_PVC_Finished);
        }

        //  refresh imu
        vec_cali.add_sample(imu.get_acceleration_vec());
        SLEEP(200);
    }

    STATE(ML_PVC_Finished) {
        if (!BTN_CBPCH) TO(ML_Active);
    }

    STATE(ML_Active) {
        if (BTN_CBGV) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_GPVC_2);
        }
        if (BTN_CBPCH) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_PVC_2);
        }
        if (BTN_SAVE) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_SaveC);
        }
    }

    STATE(ML_GPVC_2) {
        if (!BTN_CBGV) TO(ML_Active);

        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) TO(ML_GPVC_Start);
    }

    STATE(ML_PVC_2) {
        if (!BTN_CBPCH) TO(ML_Active);

        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) TO(ML_PVC_Start);
    }

    STATE(ML_SaveC) {
        if (!BTN_SAVE) TO(ML_Active);

        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 5000) {
            //  Save calibration ot EEPROM
            Vector3D gravity_vec = pitch_reading.get_gravity_calibration();
            Vector3D pitch_vec = pitch_reading.get_pitch_direction_calibration();
            float depth = pressure_sensor.get_depth_calibration();

            SavedCalibration calibration;

            calibration.x_gravity = gravity_vec.get_x();
            calibration.y_gravity = gravity_vec.get_y();
            calibration.z_gravity = gravity_vec.get_z();

            calibration.x_pitch = pitch_vec.get_x();
            calibration.y_pitch = pitch_vec.get_y();
            calibration.z_pitch = pitch_vec.get_z();

            calibration.depth_zero = depth;

            save_calibration_to_eeprom(calibration);

            //  Save the version number to EEPROM

            EEPROM.put(ADDR_EEPROM_VERSION, EEPROM_VERSION);

            TO(ML_SaveC_Finished);
        }
    }

    STATE(ML_SaveC_Finished) {
        if (!BTN_SAVE) TO(ML_Active);
    }

}

//  +----------------------------- Main Display Interfaces -----------------------------+

void main_display_interface_update() {
    SETUP_FSM_FUNCTION(MAIN_DISPLAY_INTERFACE)

    STATE(0) {
        //  initializations
        TO_NEXT;
    }

    STATE(1) {
        //  Green Light 1
        switch (GET_STATE(MAIN_LOOP)){
            case ML_GPVC_0:
            case ML_GPVC_1:
            case ML_GPVC_2:
            case ML_SaveC:
                hud.set_green_led(0, CBS_ON);
                break;

            case ML_Initialization:
            case ML_Idle_0:
                hud.set_green_led(0, CBS_BLINK_SLOW);
                break;

            case ML_GPVC_Start:
            case ML_GPVC_Reading:
            case ML_SaveC_Finished:
                hud.set_green_led(0, CBS_BLINK_FAST);
                break;
            
            default:
                hud.set_green_led(0, CBS_OFF);
                break;
        }
        
        //  Green Light 2
        switch (GET_STATE(MAIN_LOOP)){
            case ML_PVC_1:
            case ML_PVC_2:
            case ML_SaveC:
                hud.set_green_led(1, CBS_ON);
                break;

            case ML_Initialization:
            case ML_Idle_0:
            case ML_GPVC_0:
            case ML_GPVC_1:
            case ML_GPVC_Start:
            case ML_GPVC_Finished:
            case ML_GPVC_Reading:
            case ML_Idle_1:
                hud.set_green_led(1, CBS_BLINK_SLOW);
                break;

            case ML_PVC_Start:
            case ML_PVC_Reading:
            case ML_SaveC_Finished:
                hud.set_green_led(1, CBS_BLINK_FAST);
                break;
            
            default:
                hud.set_green_led(1, CBS_OFF);
                break;
        }
        
        SLEEP(200);
    }
}

//  +----------------------------- Pitch and Depth Display -----------------------------+

void pitch_and_depth_display_update() {
    SETUP_FSM_FUNCTION(PITCH_DEPTH_DISPLAY)

    STATE(0) {
        if (GET_STATE(MAIN_LOOP) == ML_Active) TO(1);

        SLEEP(200);
    }

    STATE(1) {
        if (GET_STATE(MAIN_LOOP) != ML_Active) TO(0);
        
        //  depth
        for (int i0 = 0; i0 < DEPTH_DISPLAY_CONFIG_COUNT; i0++) {
            if (
                DEPTH_DISPLAY_CONFIG[i0].lower_range <= pressure_sensor.get_depth_m() 
                && pressure_sensor.get_depth_m() <= DEPTH_DISPLAY_CONFIG[i0].upper_range
            ) {
                for (int i1 = 0; i1 < 5; i1++) {
                    hud.set_red_led(i1, DEPTH_DISPLAY_CONFIG[i0].display[i1]);
                }
                break;
            }
        }

        //  pitch
        for (int i0 = 0; i0 < PITCH_DISPLAY_CONFIG_COUNT; i0++) {
            if (
                PITCH_DISPLAY_CONFIG[i0].lower_range <= pitch_reading.get_pitch_deg()
                && pitch_reading.get_pitch_deg() <= PITCH_DISPLAY_CONFIG[i0].upper_range
            ) {
                for (int i1 = 0; i1 < 5; i1++) {
                    hud.set_yellow_led(i1, PITCH_DISPLAY_CONFIG[i0].display[i1]);
                }
                break;
            }
        }

        SLEEP(200);
    }
}

//  +------------------------- Blinking Builtin LED, Status LED ------------------------+

void blink_update() {
    SETUP_FSM_FUNCTION(BLINK);

    STATE(0) {
        pinMode(PIN_LED_BUILTIN, OUTPUT);
        TO_NEXT;
    }

    STATE(1) {
        digitalWrite(PIN_LED_BUILTIN, HIGH);
        
        if (GET_STATE(MAIN_LOOP) == ML_Active) {
            SLEEP_TO_NEXT(DELAY_ACTIVE_LED_BUILTIN);
        } else {
            SLEEP_TO_NEXT(DELAY_IDLE_LED_BUILTIN);
        }   
    }

    STATE(2) {
        digitalWrite(PIN_LED_BUILTIN, LOW);
        if (GET_STATE(MAIN_LOOP) == ML_Active) {
            SLEEP_TO(DELAY_ACTIVE_LED_BUILTIN, 1);
        } else {
            SLEEP_TO(DELAY_IDLE_LED_BUILTIN, 1);
        }   
    }
}

//  

void logging_update() {
    SETUP_FSM_FUNCTION(LOGGING)

    STATE(0) {
        // init
        if (GET_STATE(MAIN_LOOP) == ML_Active) TO_NEXT;
    }

    STATE(1) {
        if (GET_STATE(MAIN_LOOP) != ML_Active) TO(0);
        Serial.print(GET_STATE(MAIN_LOOP));
        Serial.print(" -p ");
        Serial.print(pitch_reading.get_pitch_deg());
        Serial.print("; -d ");
        Serial.print(pressure_sensor.get_depth_m());
        Serial.println();
        SLEEP(1000);
    }
}

#elif defined(SYSTEM_TEST)
//  +---------------------------------------------------------------------------------------------+
//  |                                         System Test                                         |
//  +---------------------------------------------------------------------------------------------+
void setup() {
    Serial.begin(9600);

    pinMode(13, OUTPUT);

    pinMode(PIN_BTN_CBGVT, INPUT_PULLUP);
    pinMode(PIN_BTN_CBPCH, INPUT_PULLUP);
    pinMode(PIN_BTN_SAVE,  INPUT_PULLUP);
}

void loop() {
    Serial.print(digitalRead(PIN_BTN_CBGVT));
    Serial.print(digitalRead(PIN_BTN_CBPCH));
    Serial.print(digitalRead(PIN_BTN_SAVE));
    Serial.println();

    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
}

#endif 
