#include "main.h"
#if defined(ELECTRONIC_SYSTEM)
//  +---------------------------------------------------------------------------------------------+
//  |                                      Electronic System                                      |
//  +---------------------------------------------------------------------------------------------+

void setup() {
    Serial.begin(9600);

    imu.set_refresh_period_ms(200);
    imu.begin();

    pitch_reading.set_refresh_period_ms(200);
    pitch_reading.register_imu(&imu);

    pressure_sensor.begin();

    hud.set_fast_blink_period_ms(250);  //  2Hz
    hud.set_slow_blink_period_ms(500);  //  1Hz
}

void loop() {
    hud.update();

    if (GET_STATE(MAIN_LOOP) == ML_Active) {
        imu.update();
        pitch_reading.update();
        pressure_sensor.update();
    }

    blink_update();
    main_loop_update();
}

//  +------------------------------- Background Services -------------------------------+

//  +------------------------------------ Main Loop ------------------------------------+

void main_loop_update() {
    SETUP_FSM(MAIN_LOOP)

    STATE(ML_Initialization) {
        Serial.println("Hello");
        SLEEP(1000);
    }
}

//  +------------------------- Blinking Builtin LED, Status LED ------------------------+

void blink_update() {
    SETUP_FSM(BLINK);

    STATE(0) {
        pinMode(PIN_LED_BUILTIN, OUTPUT);
        TO_NEXT;
    }

    STATE(1) {
        digitalWrite(PIN_LED_BUILTIN, HIGH);
        SLEEP_TO_NEXT(DELAY_INTERVAL_LED_BUILTIN);
    }

    STATE(2) {
        digitalWrite(PIN_LED_BUILTIN, LOW);
        SLEEP_TO(DELAY_INTERVAL_LED_BUILTIN, 1);
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
