#include "Charlieplex4Pin.h"

void Charlieplex4Pin::led_on(
    int pin_high, int pin_low, int pin_disconnect_1, int pin_disconnect_2
)const {
    pinMode(pin_array[pin_high], OUTPUT);
    digitalWrite(pin_array[pin_high], HIGH);

    pinMode(pin_array[pin_low], OUTPUT);
    digitalWrite(pin_array[pin_low], LOW);

    pinMode(pin_array[pin_disconnect_1], INPUT);
    pinMode(pin_array[pin_disconnect_2], INPUT);
}

void Charlieplex4Pin::led_on_permutation(int i) const{
    led_on(
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][0],
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][1],
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][2],
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][3]
    );
}

Charlieplex4Pin::Charlieplex4Pin(int pin0, int pin1, int pin2, int pin3)
    : Charlieplex4Pin(pin0, pin1, pin2, pin3, 2000) {
}

Charlieplex4Pin::Charlieplex4Pin(
    int pin0, int pin1, int pin2, int pin3, int refresh_period_microsecond
)   : refresh_period_us(refresh_period_microsecond) {

    pin_array[0] = pin0;
    pin_array[1] = pin1;
    pin_array[2] = pin2;
    pin_array[3] = pin3;
}

Charlieplex4Pin::~Charlieplex4Pin() {
}

void Charlieplex4Pin::update() {
    unsigned long curr_time = micros();
    if (
        (this->prev_timestamp_us + this->refresh_period_us <= curr_time) 
        || (curr_time < this->prev_timestamp_us)   //  detect micros() overflow
    ) {
        // Serial.println(curr_time);
        this->prev_timestamp_us = curr_time;    //  record the new timestamp

        this->refresh();
    }
}

void Charlieplex4Pin::refresh() const {
    for (int i = 0; i < 12; i++) {
        if (led_states[i]) {
            this->led_on_permutation(i);
            delayMicroseconds(this->led_on_delay_us);
        }
    }

    //  turn off all pins, prevent from inconsistent brightness
    for (int i = 0; i < 4; i++) {
        digitalWrite(pin_array[i], LOW);
    }
}

void Charlieplex4Pin::set_led_on_delay(unsigned long microsecond) {
    this->led_on_delay_us = microsecond;
}

void Charlieplex4Pin::set_refresh_period(unsigned long microsecond) {
    this->refresh_period_us = microsecond;
}

void Charlieplex4Pin::set_led_state(int index_permutation, bool state) {
    this->led_states[index_permutation] = state;
}

bool Charlieplex4Pin::get_led_state(int index_permutation) const {
    return this->led_states[index_permutation];
}

void Charlieplex4Pin::set_green_led(int index, bool state) {
    this->led_states[CHARLIEPLEX_LED_GREEN[index]] = state;
}

void Charlieplex4Pin::set_red_led(int index, bool state) {
    this->led_states[CHARLIEPLEX_LED_RED[index]] = state;
}

void Charlieplex4Pin::set_yellow_led(int index, bool state) {
    this->led_states[CHARLIEPLEX_LED_YELLOW[index]] = state;
}

void Charlieplex4Pin::clear_states() {
    for (int i = 0; i < 12; i++) {
        this->led_states[i] = false;
    }
}
