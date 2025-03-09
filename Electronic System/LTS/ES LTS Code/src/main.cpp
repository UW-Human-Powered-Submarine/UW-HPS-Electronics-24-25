#include "main.h"
#if defined(ELECTRONIC_SYSTEM)
//  +---------------------------------------------------------------------------------------------+
//  |                                      Electronic System                                      |
//  +---------------------------------------------------------------------------------------------+

void setup() {

}

void loop() {
    FSM_TICK(BLINK)
}


//  +-----------------------------------------------------------------------------------+
//  |                          Blinking Builtin LED, Status LED                         |
//  +-----------------------------------------------------------------------------------+
FSM_TICK_FUNCTION_W_SCHEDULER(BLINK, 

    //  Initialization BLINK
    FSM_MOORE_EVENT(0, 
        pinMode(PIN_LED_BUILTIN, OUTPUT);
    )
    FSM_TRANSITION(0, 1, true)

    //  Turn the LED ON
    FSM_MOORE_EVENT(1, 
        digitalWrite(PIN_LED_BUILTIN, HIGH);
        FSM_SCHDR_SLEEP_TRNS(DELAY_INTERVAL_LED_BUILTIN, 2)
    )

    //  Turn the LED off
    FSM_MOORE_EVENT(2, 
        // Serial.println("here2");
        digitalWrite(PIN_LED_BUILTIN, LOW);
        FSM_SCHDR_SLEEP_TRNS(DELAY_INTERVAL_LED_BUILTIN, 1)
    )
)


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
