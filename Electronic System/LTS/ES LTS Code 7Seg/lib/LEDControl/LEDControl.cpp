#include "LEDControl.h"

LEDControl::LEDControl(int pin)
    : Scheduler(10000)
    , pin(pin) {
}

void LEDControl::begin() {
    pinMode(pin, OUTPUT);
}

void LEDControl::on() {
    this->half_period_ms = 0; 
    digitalWrite(this->pin, HIGH);
}

void LEDControl::off() {
    this->half_period_ms = 0; 
    digitalWrite(this->pin, LOW);
}

void LEDControl::blink(int period_ms) {
    this->half_period_ms = period_ms / 2; 
}

void LEDControl::event() {
    SETUP_FSM_FUNCTION(LED);

    STATE(0) {
        //  OFF state
        if (half_period_ms == 0) {
            TO(2);
        }

        digitalWrite(this->pin, LOW);
        SLEEP_TO(this->half_period_ms, 1);
    }

    STATE(1) {
        //  ON state
        if (half_period_ms == 0) {
            TO(2);
        }

        digitalWrite(this->pin, HIGH);
        SLEEP_TO(this->half_period_ms, 0);
    }

    STATE(2) {
        //  Constant state
        if (half_period_ms > 0) {
            TO(0);
        }
    }
}