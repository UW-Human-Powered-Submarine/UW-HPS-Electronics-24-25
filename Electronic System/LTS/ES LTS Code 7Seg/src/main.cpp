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
    text_scroll_counter = 0;

    led_builtin.begin();
    led_gravity_calibration.begin();
    led_pitch_calibration.begin();
    led_epprom_save.begin();
}

void loop() {
    imu.update();
    ms5873.update();
    segmented_display.update();

    if (GET_STATE(MAIN_LOOP) == ML_Active) {
        pitch_reading.update();
        depth_reading.update();
    } 

    led_builtin.update();
    led_gravity_calibration.update();
    led_pitch_calibration.update();
    led_epprom_save.update();

    main_loop_update();
    hud_interface_update();
    status_led_update();

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

void erase_eeprom() {
    //  Change EEPROM version number, then the saved calibration is invalidated.
    EEPROM.put(ADDR_EEPROM_VERSION, EEPROM_VERSION + 1);   
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

        if (BTN_CBGV || BTN_CBPCH) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_EraseC);
        }

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

    STATE(ML_EraseC) {
        if (!(BTN_CBGV || BTN_CBPCH)) {
            TS_REFRESH(MAIN_LOOP_TIMER);
            TO(ML_SaveC);
        }

        if (TS_TIME_ELAPSED_MS(MAIN_LOOP_TIMER) >= 5000) {
            erase_eeprom();
            TO(ML_EraseC_Finished);
        }
    }

    STATE(ML_EraseC_Finished) {
        if (!(BTN_CBGV || BTN_CBPCH)) TO(ML_Idle_0);
    }
}
//  +---------------------------------- HUD Interface ----------------------------------+

void hud_interface_update() {
    SETUP_FSM_FUNCTION(HUD_INTERFACE_UPDATE);

    STATE(HIU_Welcome) {
        switch (text_scroll_counter / HUD_DISPLAY_TEXT_DELAY_MULTIPLIER) {
            case 0:
            case 1: segmented_display.set_static_text("UW HPS");      break;
            case 2: 
            case 3: segmented_display.set_static_text("E-SYS ");      break;
            case 4: 
            case 5: segmented_display.set_static_text("2025  ");      break;
            case 6: 
            case 7: segmented_display.set_static_text(HUD_EMPTY_MSG); break;

            default:
                text_scroll_counter = 0;
                TO(HIU_HUB_Reset);
                break;
        }
        
        text_scroll_counter++;
        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_HUB) {
        switch (GET_STATE(MAIN_LOOP)) {
            case ML_Active: 
                TO(HIU_Sensor_Info); 
                break;

            case ML_Idle_0: 
                TO(HIU_Idle_0); 
                break;

            case ML_GPVC_0: 
            case ML_GPVC_1: 
            case ML_GPVC_2: 
                TO(HIU_GPVC); 
                break;

            case ML_Idle_1: 
                TO(HIU_Idle_1); 
                break;

            case ML_PVC_1: 
            case ML_PVC_2: 
                TO(HIU_PVC); 
                break;

            case ML_GPVC_Start: 
            case ML_GPVC_Reading: 
            case ML_PVC_Start: 
            case ML_PVC_Reading: 
                TO(HIU_Reading); 
                break;

            case ML_GPVC_Finished:
            case ML_PVC_Finished:
            case ML_SaveC_Finished:
            case ML_EraseC_Finished:
                TO(HIU_Finish); 
                break;

            case ML_SaveC: 
                TO(HIU_Save); 
                break;

            case ML_EraseC: 
                TO(HIU_Erase); 
                break;
            
            default:
                break;
        }
    }

    STATE(HIU_HUB_Reset) {
        segmented_display.set_static_text(HUD_EMPTY_MSG);
        segmented_display.set_static_text_blink_mode(SDBS_ON);
        text_scroll_counter = 0;
        TO(HIU_HUB);
    }

    STATE(HIU_Sensor_Info) {
        if (GET_STATE(MAIN_LOOP) != ML_Active) TO(HIU_HUB_Reset);

        segmented_display.set_depth(depth_reading.get_depth_m());
        for (int i = 0; i < DEPTH_DISPLAY_CONFIG_COUNT; i++) {
            if (
                (DEPTH_DISPLAY_CONFIG[i].lower_range <= depth_reading.get_depth_m()) 
                && (depth_reading.get_depth_m() <= DEPTH_DISPLAY_CONFIG[i].upper_range)
            ) {
                segmented_display.set_depth_blink_mode(DEPTH_DISPLAY_CONFIG[i].status);
                break;
            }
        }

        segmented_display.set_pitch(pitch_reading.get_pitch_deg());
        for (int i = 0; i < PITCH_DISPLAY_CONFIG_COUNT; i++) {
            if (
                (PITCH_DISPLAY_CONFIG[i].lower_range <= pitch_reading.get_pitch_deg()) 
                && (pitch_reading.get_pitch_deg() <= PITCH_DISPLAY_CONFIG[i].upper_range)
            ) {
                segmented_display.set_pitch_blink_mode(PITCH_DISPLAY_CONFIG[i].status);
                break;
            }
        }

        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_Idle_0) {
        if (GET_STATE(MAIN_LOOP) != ML_Idle_0) TO(HIU_HUB_Reset);
        
        switch (text_scroll_counter / HUD_DISPLAY_TEXT_DELAY_MULTIPLIER) {
            case 0: segmented_display.set_static_text(" ZERO ");      break;
            case 1: segmented_display.set_static_text("  AT  ");      break;
            case 2: segmented_display.set_static_text("NEUTRL");      break;
            case 3: segmented_display.set_static_text("  AT  ");      break;
            case 4: segmented_display.set_static_text("SURFCE");      break;
            case 5: segmented_display.set_static_text(HUD_EMPTY_MSG); break;

            default:
                text_scroll_counter = 0;
                break;
        }
        
        text_scroll_counter++;
        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_GPVC) {
        if (
            GET_STATE(MAIN_LOOP) != ML_GPVC_0
            && GET_STATE(MAIN_LOOP) != ML_GPVC_1
            && GET_STATE(MAIN_LOOP) != ML_GPVC_2
        ) TO(HIU_HUB_Reset);
        
        segmented_display.set_static_text("ZERO 1"); 

        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_Idle_1) {
        if (GET_STATE(MAIN_LOOP) != ML_Idle_1) TO(HIU_HUB_Reset);
        
        switch (text_scroll_counter / HUD_DISPLAY_TEXT_DELAY_MULTIPLIER) {
            case 0: segmented_display.set_static_text(" ZERO ");      break;
            case 1: segmented_display.set_static_text("  AT  ");      break;
            case 2: segmented_display.set_static_text("APPROX");      break;
            case 3: segmented_display.set_static_text("30 DEG");      break;
            case 4: segmented_display.set_static_text(HUD_EMPTY_MSG); break;

            default:
                text_scroll_counter = 0;
                break;
        }
        
        text_scroll_counter++;
        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_PVC) {
        if (
            GET_STATE(MAIN_LOOP) != ML_PVC_1
            && GET_STATE(MAIN_LOOP) != ML_PVC_2
        ) TO(HIU_HUB_Reset);
        
        segmented_display.set_static_text("ZERO 2"); 

        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_Reading) {
        if (
            GET_STATE(MAIN_LOOP) != ML_GPVC_Start
            && GET_STATE(MAIN_LOOP) != ML_GPVC_Reading
            && GET_STATE(MAIN_LOOP) != ML_PVC_Start
            && GET_STATE(MAIN_LOOP) != ML_PVC_Reading
        ) TO(HIU_HUB_Reset);
        
        segmented_display.set_static_text(" HOLD "); 

        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_Finish) {
        if (
            GET_STATE(MAIN_LOOP) != ML_GPVC_Finished
            && GET_STATE(MAIN_LOOP) != ML_PVC_Finished
            && GET_STATE(MAIN_LOOP) != ML_SaveC_Finished
            && GET_STATE(MAIN_LOOP) != ML_EraseC_Finished
        ) TO(HIU_HUB_Reset);
        
        segmented_display.set_static_text("FINISH"); 

        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_Save) {
        if (GET_STATE(MAIN_LOOP) != ML_SaveC) TO(HIU_HUB_Reset);
        
        segmented_display.set_static_text(" SAUE "); 

        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }

    STATE(HIU_Erase) {
        if (GET_STATE(MAIN_LOOP) != ML_EraseC) TO(HIU_HUB_Reset);
        
        segmented_display.set_static_text("ERASE "); 
        
        SLEEP(HUD_DISPLAY_REFRESH_PERIOD);
    }
}

//  +------------------------------------ Status LED -----------------------------------+

void status_led_update() {
    SETUP_FSM_FUNCTION(STATUS_LED);

    STATE(0) TO_NEXT;

    STATE(1) {
        switch (GET_STATE(MAIN_LOOP)) {
            case ML_Idle_0:
                led_gravity_calibration.blink(DELAY_STATUS_LED_SLOW_BLINK);
                led_pitch_calibration.off();
                led_epprom_save.off();
                break;
    
            case ML_GPVC_0:
            case ML_GPVC_1:
            case ML_GPVC_2:
            case ML_GPVC_Finished:
                led_gravity_calibration.on();
                led_pitch_calibration.off();
                led_epprom_save.off();
                break;
    
            case ML_GPVC_Start:
            case ML_GPVC_Reading:
                led_gravity_calibration.blink(DELAY_STATUS_LED_FAST_BLINK);
                led_pitch_calibration.off();
                led_epprom_save.off();
                break;
    
            case ML_Idle_1:
                led_gravity_calibration.off();
                led_pitch_calibration.blink(DELAY_STATUS_LED_SLOW_BLINK);
                led_epprom_save.off();
                break;
    
            case ML_PVC_1:
            case ML_PVC_2:
            case ML_PVC_Finished:
                led_gravity_calibration.off();
                led_pitch_calibration.on();
                led_epprom_save.off();
                break;
    
            case ML_PVC_Start:
            case ML_PVC_Reading:
                led_gravity_calibration.off();
                led_pitch_calibration.blink(DELAY_STATUS_LED_FAST_BLINK);
                led_epprom_save.off();
                break;

            case ML_SaveC:
                led_gravity_calibration.off();
                led_pitch_calibration.off();
                led_epprom_save.on();
                break;

            case ML_EraseC:
                led_gravity_calibration.blink(DELAY_STATUS_LED_FAST_BLINK);
                led_pitch_calibration.blink(DELAY_STATUS_LED_FAST_BLINK);
                led_epprom_save.on();
                break;

            case ML_EraseC_Finished:
                led_gravity_calibration.on();
                led_pitch_calibration.on();
                led_epprom_save.on();
                break;
            
            default:
                led_gravity_calibration.off();
                led_pitch_calibration.off();
                led_epprom_save.off();
                break;
        }

        SLEEP(100);
    }
}

//  +-------------------------------- Blinking Builtin LED -----------------------------+

void blink_update()
{
    SETUP_FSM_FUNCTION(BLINK);

    STATE(0) {
        //  hub
        if (GET_STATE(MAIN_LOOP) == ML_Active) {
            led_builtin.blink(DELAY_ACTIVE_LED_BUILTIN);
            TO(1);
        }

        SLEEP(100);
    }

    STATE(1) {
        //  when main is at Active
        if (GET_STATE(MAIN_LOOP) != ML_Active) {
            led_builtin.blink(DELAY_IDLE_LED_BUILTIN);
            TO(0);
        }

        SLEEP(100);
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
