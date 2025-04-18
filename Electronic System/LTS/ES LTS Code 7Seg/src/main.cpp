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

    depth_reading.set_refresh_period_ms(200);
    depth_reading.register_ms5873(&ms5873);
    depth_reading.in_fresh_water();

    segmented_display.set_refresh_period_ms(200);
    segmented_display.begin();
}

void loop() {
    imu.update();
    ms5873.update();
    segmented_display.update();

    if (GET_STATE(MAIN_LOOP) == ML_Active) {
        pitch_reading.update();
        depth_reading.update();
    } 

    main_loop_update();
    hud_interface_update();

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
    //  Save the version number to EEPROM
    EEPROM.put(ADDR_EEPROM_VERSION, EEPROM_VERSION);

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
            depth_reading.calibrate_depth_zero(calibration.depth_zero);

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

        TO(ML_GPVC_Reading);
    }

    STATE(ML_GPVC_Reading) {
        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 2000) {
            pitch_reading.calibrate_gravity(vec_cali.get_sample_average());
            depth_reading.calibrate_depth_zero(depth_cali.get_sample_average());
            TO(ML_GPVC_Finished);
        }

        vec_cali.add_sample(imu.get_acceleration_vec());
        depth_cali.add_sample(ms5873.depth());

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
            float depth = depth_reading.get_depth_calibration();

            SavedCalibration calibration;

            calibration.x_gravity = gravity_vec.get_x();
            calibration.y_gravity = gravity_vec.get_y();
            calibration.z_gravity = gravity_vec.get_z();

            calibration.x_pitch = pitch_vec.get_x();
            calibration.y_pitch = pitch_vec.get_y();
            calibration.z_pitch = pitch_vec.get_z();

            calibration.depth_zero = depth;

            save_calibration_to_eeprom(calibration);

            TO(ML_SaveC_Finished);
        }
    }

    STATE(ML_SaveC_Finished) {
        if (!BTN_SAVE) TO(ML_Active);
    }

}
//  +---------------------------------- HUD Interface ----------------------------------+

void hud_interface_update() {
    SETUP_FSM_FUNCTION(HUD_INTERFACE_UPDATE);

    STATE(0) {
        if (GET_STATE(MAIN_LOOP) == ML_Active) {
            segmented_display.set_depth(depth_reading.get_depth_m());
            segmented_display.set_pitch(pitch_reading.get_pitch_deg());
            SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
        }
    }
}


//  +------------------------- Blinking Builtin LED, Status LED ------------------------+

void blink_update()
{
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

//  +------------------------------------- Logging -------------------------------------+

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
        Serial.print(depth_reading.get_depth_m());
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
